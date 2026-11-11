// pet_xp_addition: Kyubu port of Intybyte's CubeWorld-ExpBuff-PetAddition.
//
// XP earned by a pet is multiplied by (player level / pet level) when the pet is behind its owner. A level 200 player
// with a level 50 pet gives the pet 4x the XP of a kill.
//
// At 0x5A0D39 the game executes `add [edi+0x194], eax` (EDI = the creature that gains the XP, EAX = the XP). The
// mid-function hook rewrites EAX before that instruction runs. Compatible with exp_boost: that mod scales the kill XP
// upstream and this one only corrects it for pets.
//
// The original saved FPU/SSE state by hand around its call; Kyubu's mid hook does that for us.
#include "../../common.hpp"
#include "cubeworld/cubeworld.h"

#include <algorithm>

namespace {

kyubu::Mod* g_mod = nullptr;

const uint32_t kGuidOffset = 0x08;     // Creature: 64-bit id
const uint32_t kLevelOffset = 0x190;   // Creature: level
const uint32_t kParentOffset = 0x198;  // Creature: id of the owner (parent_owner), 0 for a free creature

uintptr_t local_player() {
    const uintptr_t global = g_mod->symbol("cube::game_controller");
    uint32_t gc = 0;
    if (!global || g_mod->api()->mem_read(global, &gc, sizeof gc) != KY_OK || !gc) return 0;
    uint32_t player = 0;
    if (g_mod->api()->mem_read(gc + offsetof(cube_GameController, player), &player, sizeof player) != KY_OK) return 0;
    return player;
}

void KYUBU_CALL on_pet_xp(KyubuCpuContext* ctx, void*) {
    const KyubuApi* api = g_mod->api();
    const uintptr_t pet = ctx->edi;
    const uintptr_t player = local_player();
    if (!pet || !player) return;

    int64_t player_guid = 0, pet_parent = 0;
    int32_t player_level = 0, pet_level = 0;
    if (api->mem_read(player + kGuidOffset, &player_guid, sizeof player_guid) != KY_OK ||
        api->mem_read(pet + kParentOffset, &pet_parent, sizeof pet_parent) != KY_OK)
        return;
    if (player_guid != pet_parent) return;   // not the local player's pet: keep the vanilla XP

    if (api->mem_read(player + kLevelOffset, &player_level, sizeof player_level) != KY_OK ||
        api->mem_read(pet + kLevelOffset, &pet_level, sizeof pet_level) != KY_OK || pet_level <= 0)
        return;

    const float ratio = std::max(static_cast<float>(player_level) / static_cast<float>(pet_level), 1.0f);
    ctx->eax = static_cast<uint32_t>(static_cast<int32_t>(static_cast<float>(static_cast<int32_t>(ctx->eax)) * ratio));
}

}  // namespace

struct PetXpAddition {
    explicit PetXpAddition(kyubu::Mod& m) {
        g_mod = &m;
        const uintptr_t site = classic::at(m, 0x1A0D39);
        static const uint8_t want[] = {0x01, 0x87, 0x94, 0x01, 0x00, 0x00};
        if (!classic::expect(m, site, want, sizeof want, "pet_xp_addition")) return;
        KyubuId id = 0;
        const KyubuResult r = m.api()->hook_mid(m.handle(), site, &on_pet_xp, nullptr, &id);
        if (r != KY_OK) { m.error("pet_xp_addition: could not hook the XP add (result %d)", static_cast<int>(r)); return; }
        m.info("pet_xp_addition: pets behind their owner earn more XP");
    }
};

KYUBU_MOD("pet_xp_addition", KY_SIDE_CLIENT, PetXpAddition)

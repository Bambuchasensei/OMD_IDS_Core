# IDS HUD icons (approved 2026-10-29)
White silhouettes on transparent background; color is set in code via SetColor. Counter = TextWidget (like TerjeCore IngameHud.c), not images.

body/: head, torso, arm (fist), hand, leg, foot (Andrea's drawings, cleaned)
organs/: lungs, bone (Andrea's drawings, cleaned)
wounds/ (from TerjeMedicine icon_medicine atlas):
- bullet = tm_bullethit
- deep_wound = tm_wound, deep_wound_big = tm_bigwound
- bandage_clean = tm_bandagedclean_2, bandage_dirty = tm_bandageddirty_2
- suture = tm_protectingwound
- organ (общее ранение органа) = tm_viscera

Not done yet: bone fracture and splinted fracture (Terje has none), organs heart/liver/kidneys/guts.
TODO: convert to .paa/.edds + imageset before PBO; check Terje license/credits before release.

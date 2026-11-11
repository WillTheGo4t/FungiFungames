# Modular Audio System

A data-driven Audio Plugin for Unreal Engine 5. This system centralizes sound management using Data Assets, allowing programmers to trigger sounds globally via Gameplay Tags while giving sound designers full control over playback parameters, fades, and variations without touching code.

---

## 1. Programmer Workflow

Playing a sound anywhere in the project requires only a Gameplay Tag. You do not need to reference specific audio files or actors.

**How to trigger a sound in any Blueprint (Actor, Widget, Level Blueprint):**
1. Right-click anywhere in your Blueprint graph and search for the **Get MAS_BPS_AudioSystem_C** node.
2. Drag a wire from the Subsystem return pin and search for the **MAS_PlayModularSound** event.

**Required Parameter:** 
* `Sound Tag`: A Gameplay Tag

**Optional Parameters:** 
* `Location` (Vector): Used if the sound is configured to spawn at a specific world location.
* `Attach Target` (Scene Component) & `Attach Point Name`: Used if the sound should follow a moving actor.

---

## 2. Designer Workflow

All audio balancing, limits, and configurations are handled inside the `MAS_AudioConfigRegistry` Data Asset. 

1. Add a new entry to the Map and assign a unique **Gameplay Tag**.
2. Add one or multiple **SoundWaves** to the array.
3. Adjust base properties like Volume, Pitch, and Sound Class. Hover over any property in the editor to read its specific tooltip.

**Editor Preview & Troubleshooting Tip:**
You can preview your sounds directly inside the Data Asset using the Play/Stop buttons. Clicking Play will simulate the sound in your currently open level. If your current level is unsuitable for audio testing, you can open and use the provided `MAS_Lvl_AudioTester` level instead.
*   *Note:* If the preview is silent right after launching the Unreal Editor, open the `MAS_BPS_AudioSystem` blueprint once to force the engine to load the subsystem.
*   *Note:* Sounds set to `Spawn Attached` or `Spawn At Location` might sound off-center in the editor preview. Temporarily switch the Playback Type to `Play 2D` while tuning the sound in the Data Asset.

---

## 3. Extending the System

This system is modular. You can freely extend its capabilities to fit your project's needs.

**Adding a new Sound Class (e.g., UI, Voice, Ambient):**
1. Open `AudioConfigRegistry.h`.
2. Add your new class to the `ESoundClassEnum`.
3. Create corresponding Mono and Stereo MetaSounds (or SoundCues) in your project files and assign them your new Engine Sound Class.
4. Inside `MAS_BPS_AudioSystem`, update the `MSMap Stereo` and `MSMap Mono` variables to link your new Enum entry to the respective MetaSound assets.

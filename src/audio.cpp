#include <raylib.h>

Sound AUDIO_single;
Sound AUDIO_burst;
Sound AUDIO_select;
Sound AUDIO_noAmmo;
Sound AUDIO_forbidden;
Sound AUDIO_negative;
Sound AUDIO_powerup;
Sound AUDIO_damage;

void loadAudio() {
    AUDIO_single = LoadSound("ogg/single.ogg");
    AUDIO_burst = LoadSound("ogg/burst.ogg");
    AUDIO_select = LoadSound("ogg/select.ogg");
    AUDIO_noAmmo = LoadSound("ogg/noAmmo.ogg");
    AUDIO_forbidden = LoadSound("ogg/forbidden.ogg");
    AUDIO_negative = LoadSound("ogg/negative.ogg");
    AUDIO_powerup = LoadSound("ogg/powerup.ogg");
    AUDIO_damage = LoadSound("ogg/damage.ogg");
}

void unloadAudio() {
    UnloadSound(AUDIO_burst);
    UnloadSound(AUDIO_forbidden);
    UnloadSound(AUDIO_negative);
    UnloadSound(AUDIO_noAmmo);
    UnloadSound(AUDIO_select);
    UnloadSound(AUDIO_single);
    UnloadSound(AUDIO_powerup);
}

void playSingle() { PlaySound(AUDIO_single); }
void playBurst() { PlaySound(AUDIO_burst); }
void playSelect() { PlaySound(AUDIO_select); }
void playNoAmmo() { PlaySound(AUDIO_noAmmo); }
void playForbidden() { PlaySound(AUDIO_forbidden); }
void playNegative() { PlaySound(AUDIO_negative); }
void playPowerup() { PlaySound(AUDIO_powerup); }
void playDamage() { PlaySound(AUDIO_damage); }
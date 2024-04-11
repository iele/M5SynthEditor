#pragma once

#include <M5Unified.h>

#define TONE_OFFSET 27

static char *tones[] = { // offset 27
    "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
    "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
    "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
    "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7"};

static char *instruments[] = {
    "GrandPiano_1",
    "BrightPiano_2",
    "ElGrdPiano_3",
    "HonkyTonkPiano",
    "ElPiano1",
    "ElPiano2",
    "Harpsichord",
    "Clavi",
    "Celesta",
    "Glockenspiel",
    "MusicBox",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "TubularBells",
    "Santur",
    "DrawbarOrgan",
    "PercussiveOrgan",
    "RockOrgan",
    "ChurchOrgan",
    "ReedOrgan",
    "AccordionFrench",
    "Harmonica",
    "TangoAccordion",
    "AcGuitarNylon",
    "AcGuitarSteel",
    "AcGuitarJazz",
    "AcGuitarClean",
    "AcGuitarMuted",
    "OverdrivenGuitar",
    "DistortionGuitar",
    "GuitarHarmonics",
    "AcousticBass",
    "FingerBass",
    "PickedBass",
    "FretlessBass",
    "SlapBass1",
    "SlapBass2",
    "SynthBass1",
    "SynthBass2",
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "TremoloStrings",
    "PizzicatoStrings",
    "OrchestralHarp",
    "Timpani",
    "StringEnsemble1",
    "StringEnsemble2",
    "SynthStrings1",
    "SynthStrings2",
    "ChoirAahs",
    "VoiceOohs",
    "SynthVoice",
    "OrchestraHit",
    "Trumpet",
    "Trombone",
    "Tuba",
    "MutedTrumpet",
    "FrenchHorn",
    "BrassSection",
    "SynthBrass1",
    "SynthBrass2",
    "SopranoSax",
    "AltoSax",
    "TenorSax",
    "BaritoneSax",
    "Oboe",
    "EnglishHorn",
    "Bassoon",
    "Clarinet",
    "Piccolo",
    "Flute",
    "Recorder",
    "PanFlute",
    "BlownBottle",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    "Lead1Square",
    "Lead2Sawtooth",
    "Lead3Calliope",
    "Lead4Chiff",
    "Lead5Charang",
    "Lead6Voice",
    "Lead7Fifths",
    "Lead8BassLead",
    "Pad1Fantasia",
    "Pad2Warm",
    "Pad3PolySynth",
    "Pad4Choir",
    "Pad5Bowed",
    "Pad6Metallic",
    "Pad7Halo",
    "Pad8Sweep",
    "FX1Rain",
    "FX2Soundtrack",
    "FX3Crystal",
    "FX4Atmosphere",
    "FX5Brightness",
    "FX6Goblins",
    "FX7Echoes",
    "FX8SciFi",
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "BagPipe",
    "Fiddle",
    "Shanai",
    "TinkleBell",
    "Agogo",
    "SteelDrums",
    "Woodblock",
    "TaikoDrum",
    "MelodicTom",
    "SynthDrum",
    "ReverseCymbal",
    "GtFretNoise",
    "BreathNoise",
    "Seashore",
    "BirdTweet",
    "TelephRing",
    "Helicopter",
    "Applause",
    "Gunshot"};

char *tone_name(uint8_t pitch)
{
    if (pitch < 0x1B || pitch > 0x6A)
    {
        return "";
    }
    else
    {
        return tones[pitch - TONE_OFFSET];
    }
}

char *instrument_name(uint8_t instrument)
{
    if (instrument < 0 || instrument > 127)
    {
        return "";
    }
    else
    {
        return instruments[instrument];
    }
}

#define DATA_LENGTH 8
#define CHANNELS_LENGTH 256

struct Note
{
    uint8_t pitch;
    uint8_t velocity;
};

struct Channel
{
    uint8_t instrument;
    uint8_t volume;
    Note notes[CHANNELS_LENGTH] = {0};
};

class SynthData
{
public:
    void begin();

    bool setNote(uint8_t channel, uint8_t note, uint8_t pitch, uint8_t velocity);
    bool setInstrument(uint8_t channel, uint8_t instrument);
    uint8_t getNote(uint8_t channel, uint8_t note);
    uint8_t getInstrument(uint8_t channel);
    uint8_t getVolume(uint8_t channel);

    uint8_t master_volume;
    uint8_t bpm;

private:
    Channel channels[DATA_LENGTH];
};

void SynthData::begin()
{
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        for (int j = 0; j < CHANNELS_LENGTH; j++)
        {
            channels[i].notes[j].pitch = 0;
            channels[i].notes[j].velocity = 0;
        }
        channels[i].instrument = i == 0 ? 0 : i - 1;
        channels[i].volume = 127;
    }
    master_volume = 127;
    bpm = 180;
}

uint8_t SynthData::getNote(uint8_t channel, uint8_t note)
{
    if (channel >= DATA_LENGTH || note >= CHANNELS_LENGTH)
        return 0;
    return channels[channel].notes[note].pitch;
}

uint8_t SynthData::getInstrument(uint8_t channel)
{
    if (channel >= DATA_LENGTH)
        return 0;
    return channels[channel].instrument;
}

uint8_t SynthData::getVolume(uint8_t channel)
{
    if (channel >= DATA_LENGTH)
        return 0;
    return channels[channel].volume;
}

bool SynthData::setNote(uint8_t channel, uint8_t note, uint8_t pitch, uint8_t velocity)
{
    if (channel >= DATA_LENGTH || note >= CHANNELS_LENGTH)
        return false;

    if (pitch != 0x00)
    {
        if (pitch < 0x1B)
        {
            pitch = 0x6A;
        }
        if (pitch > 0x6A)
        {
            pitch = 0x1B;
        }
    }
    channels[channel].notes[note].pitch = pitch;
    channels[channel].notes[note].velocity = velocity;
    return true;
}

bool SynthData::setInstrument(uint8_t channel, uint8_t instrument)
{
    if (channel >= DATA_LENGTH)
        return false;
    if (instrument < 0)
    {
        instrument = 127;
    }
    if (instrument > 127)
    {
        instrument = 0;
    }
    channels[channel].instrument = instrument;
    return true;
}

SynthData synthData;

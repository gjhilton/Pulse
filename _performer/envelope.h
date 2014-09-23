#ifndef envelope_h
#define envelope_h

// ADSRR ENVELOPE

typedef enum {
  ENVELOPE_PHASE_ATTACK,
  ENVELOPE_PHASE_DECAY,
  ENVELOPE_PHASE_SUSTAIN,
  ENVELOPE_PHASE_RELEASE,
  ENVELOPE_PHASE_RECOVER
} EnvelopePhase;

typedef struct EnvelopeMoment {
  EnvelopePhase phase;
  float value;
} EnvelopeMoment;

EnvelopeMoment adsr(unsigned long attack,
                    unsigned long decay,
                    unsigned long sustain,
                    unsigned long release,
                    float sustain_value,
                    unsigned long elapsed)
{

  EnvelopeMoment moment;

  // sanitise input
  if (sustain_value > 1) sustain_value = 1;
  if (sustain_value < 0) sustain_value = 0;

  // attack
  if (elapsed <= attack) {
    moment.phase = ENVELOPE_PHASE_ATTACK;
    if (attack > 0) {
      moment.value = elapsed / attack;
    } else {
      moment.value = 0; // very edge case: attack = elapsed = 0
    }
    return moment;
  }

  // decay
  elapsed -= attack;
  if (elapsed <= decay) {
    moment.phase = ENVELOPE_PHASE_DECAY;
    if (sustain_value > 0) {
      moment.value = 1 - (sustain_value * (elapsed / decay));
    } else {
      moment.value = 1 - (elapsed / decay);
    }
    return moment;
  }

  // sustain
  elapsed -= decay;
  if (elapsed <= sustain) {
    moment.phase = ENVELOPE_PHASE_SUSTAIN;
    moment.value = sustain_value;
    return moment;
  }

  // release
  elapsed -= sustain;
  if (elapsed <= release) {
    moment.phase = ENVELOPE_PHASE_RELEASE;
    if (sustain > 0 && sustain_value > 0) {
      moment.value = sustain_value - (sustain_value * (elapsed / sustain));
    } else {
      moment.value = 0;
    }
    return moment;
  }

  // recovery
  moment.phase = ENVELOPE_PHASE_RECOVER;
  moment.value = 0;
  return moment;
}

// ALTERNATIVE WAVEFORMS

float triangleWave(float normalisedInput){
  float val = normalisedInput;
  if (val > 0.5){
    val = 1 - val;
  }
  return val;
}

float sawtoothWave(float normalisedInput){
  float val = normalisedInput;
  return val;
}

float sawtoothWaveReversed(float normalisedInput){
  float val = 1 - normalisedInput;
  return val;
}

#endif

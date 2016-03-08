#include "ADSR.h"

ADSR::ADSR()
    : AudioProcessor(),
      m_state(State_Off),
      m_envelope(0.0)
{
    setAttackTime(0.01);
    setDecayTime(0.1);
    setSustainLevel(0.8);
    setReleaseTime(0.15);

    m_counter = 0;
}

void ADSR::setAttackTime(double s)
{
    m_attackTime = s;
    recomputeRates();
}

void ADSR::setDecayTime(double s)
{
    m_decayTime = s;
    recomputeRates();
}

void ADSR::setSustainLevel(double s)
{
    m_sustainLevel = s;
    recomputeRates();
}

void ADSR::setReleaseTime(double s)
{
    m_releaseTime = s;
    recomputeRates();
}

void ADSR::noteOn()
{
    setState(State_Attack);
}

void ADSR::noteOff()
{
    setState(State_Release);
}

double ADSR::tickProcess(double *input, int nChannels)
{
    (void)input;
    (void)nChannels;

    process();
    return m_envelope;
}

double ADSR::lastSample(int channel) const
{
    (void)channel;

    return m_envelope;
}

void ADSR::setEnabled(bool ena)
{
    if (!ena) {
        setState(State_Off);
        m_envelope = 0.0;
    }
    AudioProcessor::setEnabled(ena);
}

void ADSR::recomputeRates()
{
    double sr = sampleRate();
    m_attackRate = 1.0 / (m_attackTime * sr);
    m_decayRate = (m_sustainLevel - 1.0) / (m_decayTime * sr);

    // The release rate is fixed based on the sustain level,
    // but the actual release time may vary depending on the
    // envelope level at the moment the resease state is requested.
    m_releaseRate = -m_sustainLevel / (m_releaseTime * sr);
}

void ADSR::process()
{
    m_counter++;

    switch (m_state) {
    case State_Attack:
        m_envelope = m_attackRate * m_counter;
        if (m_envelope >= 1.0) {
            m_envelope = 1.0;
            setState(State_Decay);
        }
        break;
    case State_Decay:
        m_envelope = 1.0 + (m_decayRate * m_counter);
        if (m_envelope <= m_sustainLevel) {
            m_envelope = m_sustainLevel;
            setState(State_Sustain);
        }
        break;
    case State_Sustain:
        // Just keep the current level
        break;
    case State_Release:
        m_envelope = m_releaseLevel + m_releaseRate * m_counter;
        if (m_envelope <= 0.0) {
            m_envelope = 0.0;
            setState(State_Off);
        }
        break;
    case State_Off:
    default:
        break;
    }
}

void ADSR::setState(State s)
{
    if (s == State_Release) {
        // Capture the current envelope level for the release phase.
        m_releaseLevel = m_envelope;
    }
    m_counter = 0;
    m_state = s;
}

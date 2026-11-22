#pragma once

#include <JuceHeader.h>
#include <array>

class OptoTubeLA2AAudioProcessor : public juce::AudioProcessor
{
public:
    OptoTubeLA2AAudioProcessor();
    ~OptoTubeLA2AAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(
        juce::AudioBuffer<float>& buffer,
        juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(
        int index,
        const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;

private:

    struct ChannelState
    {
        float detectorEnvelope = 0.0f;
        float opticalEnvelope = 0.0f;
        float harmonicEnvelope = 0.0f;
        float gainReduction = 1.0f;

        float releaseFast = 0.0f;
        float releaseSlow = 0.0f;

        float previousInput = 0.0f;
        float previousOutput = 0.0f;
    };

    std::array<ChannelState, 2> channelStates;

    double currentSampleRate = 48000.0;
    float harmonicEnvelopeCoefficient = 0.0f;

    float attackCoefficient = 0.0f;
    float releaseFastCoefficient = 0.0f;
    float releaseSlowCoefficient = 0.0f;

    float currentGain = 0.321869f;
    float currentPeakReduction = 0.277466f;

    bool limitMode = false;

    void updateTimeCoefficients();

    void updateParameters();

    float processDetector(
        float input,
        ChannelState& state);

    float processOpticalCell(
        float detectorLevel,
        ChannelState& state);

    float calculateCompressionGain(
        float detectorLevel,
        float peakReduction,
        bool isLimitMode);

    float processTubeNonlinearity(
        float input,
        ChannelState& state,
        float gain);

    float processSample(
        float input,
        ChannelState& state);

    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        OptoTubeLA2AAudioProcessor)
};

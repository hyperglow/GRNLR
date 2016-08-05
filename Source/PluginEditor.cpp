/*
 ==============================================================================

 This file was auto-generated by the Introjucer!

 It contains the basic framework code for a JUCE plugin editor.

 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
class GrnlrAudioProcessorEditor::ParameterSlider : public Slider,
                                                         private Timer
{
public:
    ParameterSlider (AudioProcessorParameter& p) : Slider (p.getName (256)), param (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (60);
        updateSliderPos();
    }

    void valueChanged() override
    {
        if(isDragging){
            param.setValueNotifyingHost((float) Slider::getValue());
        } else {
            param.setValue((float) Slider::getValue());
        }

    }

    bool isDragging = false;

    void timerCallback() override       { updateSliderPos(); }

    void startedDragging() override     { param.beginChangeGesture(); isDragging = true;  }
    void stoppedDragging() override     { param.endChangeGesture();   isDragging = false; }

    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }

    void updateSliderPos()
    {
        const float newValue = param.getValue();

        if (newValue != (float) Slider::getValue())
            Slider::setValue (newValue);
    }

    AudioProcessorParameter& param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};

class GrnlrAudioProcessorEditor::ParameterButton : public TextButton,
                                                         private Timer
{
public:
    ParameterButton (AudioProcessorParameter& p) : TextButton (p.getName (256)), param (p)
    {
        updateButton();
        startTimerHz (60);
    }

    void timerCallback() override { updateButton(); }

    void clicked() override
    {
        param.setValueNotifyingHost(TextButton::getToggleState());
    }

    void updateButton()
    {
        const bool newValue = param.getValue();

        if (newValue != TextButton::getToggleState())
            TextButton::setToggleState(newValue, sendNotification);
    }

    AudioProcessorParameter& param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterButton)

};

//==============================================================================
GrnlrAudioProcessorEditor::GrnlrAudioProcessorEditor (GrnlrAudioProcessor& p) : AudioProcessorEditor (&p),
                                                                                                  Thread("loading Thread"),
                                                                                                  processor (p)
{
    // BUTTONS
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open...");
    openButton.addListener(this);

    addAndMakeVisible(holdButton = new ParameterButton(*p.holdParam));
    holdButton->setButtonText("Hold");
    holdButton->setClickingTogglesState(true);

    // POSTITION
    addAndMakeVisible(positionSlider = new ParameterSlider (*p.positionParam));
    positionSlider->setSliderStyle(Slider::LinearBar);
    positionSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(randPosLabel);
    randPosLabel.setText("Rand Pos", dontSendNotification);

    addAndMakeVisible(randPosSlider = new ParameterSlider (*p.randPosParam));
    randPosSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    randPosSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // FILL FACTOR
    addAndMakeVisible(fillLabel);
    fillLabel.setText("Fill Factor", dontSendNotification);

    addAndMakeVisible(fillSlider = new ParameterSlider (*p.fillFactorParam));
    fillSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fillSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(randFillLabel);
    randFillLabel.setText("Rand Fill", dontSendNotification);

    addAndMakeVisible(randFillSlider = new ParameterSlider (*p.randFillParam));
    randFillSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    randFillSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // Duration
    addAndMakeVisible(durationLabel);
    durationLabel.setText("Duration", dontSendNotification);

    addAndMakeVisible(durationSlider = new ParameterSlider (*p.durationParam));
    durationSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    durationSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    //durationSlider->setSkewFactorFromMidPoint(1.5);

    addAndMakeVisible(randDurLabel);
    randDurLabel.setText("Rand Dur", dontSendNotification);

    addAndMakeVisible(randDurSlider = new ParameterSlider(*p.randDurParam));
    randDurSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    randDurSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // TRANSPOSITION
    addAndMakeVisible(transLabel);
    transLabel.setText("Transposition", dontSendNotification);

    addAndMakeVisible(transSlider = new ParameterSlider(*p.transParam));
    transSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    transSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(randTransLabel);
    randTransLabel.setText("Rand Trans", dontSendNotification);

    addAndMakeVisible(randTransSlider = new ParameterSlider(*p.randTransParam));
    randTransSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    randTransSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // ENVELOPE
    addAndMakeVisible(envCenterLabel);
    envCenterLabel.setText("Env Center", dontSendNotification);

    addAndMakeVisible(envCenterSlider = new ParameterSlider(*p.envCenterParam));
    envCenterSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    envCenterSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(envSustainLabel);
    envSustainLabel.setText("Env Sustain", dontSendNotification);

    addAndMakeVisible(envSustainSlider = new ParameterSlider(*p.envSustainParam));
    envSustainSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    envSustainSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(envCurveLabel);
    envCurveLabel.setText("Env Curve", dontSendNotification);

    addAndMakeVisible(envCurveSlider = new ParameterSlider(*p.envCurveParam));
    envCurveSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    envCurveSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // VOLUME
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", dontSendNotification);

    addAndMakeVisible(volumeSlider = new ParameterSlider(*p.volumeParam));
    volumeSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    volumeSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    addAndMakeVisible(randVolLabel);
    randVolLabel.setText("Rand Vol", dontSendNotification);

    addAndMakeVisible(randVolSlider = new ParameterSlider(*p.randVolumeParam));
    randVolSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    randVolSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // Waveform
    addAndMakeVisible(waveform = new WaveformView (formatManager, p));
    waveform->addChangeListener (this);

    // Sample handling
    formatManager.registerBasicFormats();
    startThread();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (880, 440);

    if( ! processor.loadedPath.isEmpty())
        swapVariables(chosenPath, processor.loadedPath);
}

GrnlrAudioProcessorEditor::~GrnlrAudioProcessorEditor()
{
    stopThread(4000);
}

//==============================================================================
void GrnlrAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void GrnlrAudioProcessorEditor::resized()
{
    int width = getWidth();
    Rectangle<int> r (getLocalBounds().reduced(4));

    // Waveform
    waveform->setBounds(r.removeFromTop (140));

    // Buttons
    openButton.setBounds(10, 170, 120, 20);
    holdButton->setBounds(816, 170, 60, 20);

    // Position
    positionSlider->setBounds(4, 150, width - 8, 15);
    randPosLabel.setBounds(130, 200, 70, 20);
    randPosSlider->setBounds(130, 220, 50, 65);

    // Fill Factor
    fillLabel.setBounds(10, 200, 60, 20);
    fillSlider->setBounds(10, 220, 50, 65);

    randFillLabel.setBounds(10, 300, 60, 20);
    randFillSlider->setBounds(10, 320, 50, 65);

    // Duration
    durationLabel.setBounds(70, 200, 50, 20);
    durationSlider->setBounds(70, 220, 50, 65);

    randDurLabel.setBounds(70, 300, 60, 20);
    randDurSlider->setBounds(70, 320, 50, 65);

    // Transposition
    transLabel.setBounds(190, 200, 70, 20);
    transSlider->setBounds(190, 220, 50, 65);

    randTransLabel.setBounds(190, 300, 70, 20);
    randTransSlider->setBounds(190, 320, 50, 65);

    // Envelope
    envCenterLabel.setBounds(670, 200, 70, 20);
    envCenterSlider->setBounds(670, 220, 50, 65);
    envSustainLabel.setBounds(740, 200, 70, 20);
    envSustainSlider->setBounds(740, 220, 50, 65);
    envCurveLabel.setBounds(810, 200, 70, 20);
    envCurveSlider->setBounds(810, 220, 50, 65);

    // Volume
    volumeLabel.setBounds(740, 300, 70, 20);
    volumeSlider->setBounds(740, 320, 50, 65);
    randVolLabel.setBounds(810, 300, 70, 20);
    randVolSlider->setBounds(810, 320, 50, 65);

    // Waveform
    std::cout << processor.filePath << std::endl;
    if(processor.sampleIsLoaded){
        const File file (processor.filePath);
        waveform->setFile(file);
    }

}

void GrnlrAudioProcessorEditor::buttonClicked (Button* button)
{
    if(button == &openButton) openButtonClicked();
}

void GrnlrAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

}

void GrnlrAudioProcessorEditor::run()
{
    while (! threadShouldExit())
    {
        checkForPathToOpen();
        checkForBuffersToFree();
        wait (500);
    }
}

void GrnlrAudioProcessorEditor::checkForBuffersToFree()
{
    for (int i = buffers.size(); --i >= 0;)
    {
        ReferenceCountedBuffer::Ptr buffer (buffers.getUnchecked (i));

        if (buffer->getReferenceCount() == 2)
            buffers.remove (i);
    }
}

void GrnlrAudioProcessorEditor::checkForPathToOpen()
{
    String pathToOpen;
    swapVariables (pathToOpen, chosenPath);

    if (pathToOpen.isNotEmpty())
    {
        std::cout << "We have a file!" << std::endl;
        const File file (pathToOpen);
        processor.filePath = pathToOpen;
        ScopedPointer<AudioFormatReader> reader (formatManager.createReaderFor (file));

        if (reader != nullptr)
        {
            const double duration = reader->lengthInSamples / reader->sampleRate;

            if (duration < 600)
            {
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                                   reader->numChannels,
                                                                                   reader->lengthInSamples);

                reader->read (newBuffer->getAudioSampleBuffer(), 0, reader->lengthInSamples, 0, true, true);
                processor.currentBuffer = newBuffer;
                buffers.add (newBuffer);
                processor.sampleIsLoaded  = true;
                processor.lengthInSamples = reader->lengthInSamples;
            }
            else
            {
                // handle the error that the file is 600 seconds or longer..
            }
        }
    }
}


void GrnlrAudioProcessorEditor::openButtonClicked()
{
    FileChooser chooser ( "Select a Wave file shorter than 600 seconds to play...",
                          File::nonexistent,
                          "*.wav, *.aiff, *.aif" );

    if (chooser.browseForFileToOpen())
    {
        const File file (chooser.getResult());
        String path (file.getFullPathName());
        swapVariables (chosenPath, path);
        waveform->setFile(file);
        notify();
    }
}

// This function implements a callback that the WaveformView class sends when sendChangeMessage() is
// called in the files dropped function. We use it to load the file that we have dropped in the
// WaveformView
void GrnlrAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == waveform){
        const File file(waveform->getLastDroppedFile());
        String path (file.getFullPathName());
        swapVariables(chosenPath, path);
        waveform->setFile(file);
    }
}

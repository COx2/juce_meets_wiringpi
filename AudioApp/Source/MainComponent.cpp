/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

namespace {
	const int PIN_LED1 = 15;
	const int PIN_SW1 = 4;
}

//==============================================================================
MainComponent::MainComponent()
	:WIRING_PI_ENABLE(false)
	, pinValue_SW1(0)
	, paramSwitch1(Range<int>(0, 1))
	, switch1Slider(Slider::SliderStyle::LinearBarVertical, Slider::TextEntryBoxPosition::NoTextBox)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
	
	switch1Slider.setRange(paramSwitch1.getStart(), paramSwitch1.getEnd(), 0.01);
	switch1Slider.setValue(paramSwitch1.getStart(), dontSendNotification);
	switch1Slider.setPopupDisplayEnabled(true, true, this);
	switch1Slider.setPopupMenuEnabled(true);
	addAndMakeVisible(switch1Slider);

#ifndef WIN32
	// WiringPi初期化
	if (wiringPiSetupGpio() < 0) {
		printf("GPIO ERROR\n");
		WIRING_PI_ENABLE = true;
		return;
	}
	pinMode(PIN_LED1, OUTPUT); //出力モードに切り替え
	pinMode(PIN_SW1, INPUT); //入力モードに切り替え
#endif

	startTimerHz(60);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値

	switch1Slider.setBounds(bounds.reduced(100));
}

void MainComponent::timerCallback()
{
#ifndef WIN32
	pinValue_LED1 = !pinValue_LED1;
	digitalWrite(PIN_LED1, (int)pinValue_LED1); //Lチカ

	pinValue_SW1 = digitalRead(PIN_SW1);

	switch1Slider.setValue(pinValue_SW1, dontSendNotification);
#endif // !WIN32

#ifdef WIN32
	DBG(pinValue_SW1);

	++pinValue_SW1;
	switch1Slider.setValue(pinValue_SW1, dontSendNotification);
#endif

}

#include <stdio.h>

#include <Effects/Bounce.hpp>
#include <Effects/Comet.hpp>
#include <Effects/Marquee.hpp>
#include <Effects/Particles.hpp>
#include <Effects/Stars.hpp>
#include <PicoLed.hpp>

#include "pico/stdlib.h"

#define LED_PIN 28
#define LED_LENGTH 58

int main()
{
	stdio_init_all();
	sleep_ms(5000);

	// 0. Initialize LED strip
	printf("0. Initialize LED strip\n");
	auto ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);
	// auto ledStrip = original.slice(0, 29);

	ledStrip.setBrightness(64);
	printf("1. Clear the strip!\n");

	// 1. Set all LEDs to red!
	printf("1. Set all LEDs to red!\n");
	ledStrip.fill(PicoLed::RGB(255, 0, 0));
	ledStrip.show();
	sleep_ms(500);

	// 2. Set all LEDs to green!
	printf("2. Set all LEDs to green!\n");
	ledStrip.fill(PicoLed::RGB(0, 255, 0));
	ledStrip.show();
	sleep_ms(500);

	// 3. Set all LEDs to blue!
	printf("3. Set all LEDs to blue!\n");
	ledStrip.fill(PicoLed::RGB(0, 0, 255));
	ledStrip.show();
	sleep_ms(500);

	// 4. Set half LEDs to red and half to blue!
	printf("4. Set gradient from red to blue!\n");
	ledStrip.fillGradient(PicoLed::RGB(255, 0, 0), PicoLed::RGB(0, 0, 255));
	ledStrip.show();
	sleep_ms(1000);

	// 5. Set half LEDs to red and half to blue!
	printf("5. Set rainbow colors!\n");
	ledStrip.fillRainbow(0, 255 / LED_LENGTH);
	ledStrip.show();
	sleep_ms(1000);

	// 5. Do some fancy animation
	printf("6. Do some fancy effects\n");

	// Init rainbow palette
	std::vector<PicoLed::Color> rainbowPalette;
	rainbowPalette.push_back(PicoLed::RGB(255, 0, 0));
	rainbowPalette.push_back(PicoLed::RGB(255, 255, 0));
	rainbowPalette.push_back(PicoLed::RGB(0, 255, 0));
	rainbowPalette.push_back(PicoLed::RGB(0, 255, 255));
	rainbowPalette.push_back(PicoLed::RGB(0, 0, 255));
	rainbowPalette.push_back(PicoLed::RGB(255, 0, 255));

	// Init fire palette
	std::vector<PicoLed::Color> firePalette;
	firePalette.push_back(PicoLed::RGB(0, 0, 0));
	firePalette.push_back(PicoLed::RGB(255, 0, 0));
	firePalette.push_back(PicoLed::RGB(255, 255, 0));
	firePalette.push_back(PicoLed::RGB(255, 255, 255));

	// Init effects
	PicoLed::Marquee effectMarquee(ledStrip, rainbowPalette, 5.0, -2.0, 1.0);
	PicoLed::Stars effectStars(ledStrip, PicoLed::RGB(255, 255, 255), 4.0);
	PicoLed::Comet effectComet(ledStrip, PicoLed::RGB(255, 255, 255), 10.0,
							   2.0, 1.5);
	PicoLed::Bounce effectBounce(ledStrip, 1.5, 8.0);
	effectBounce.addBall(PicoLed::RGB(255, 0, 0), 2.0);
	effectBounce.addBall(PicoLed::RGB(0, 255, 0), 2.0);
	effectBounce.addBall(PicoLed::RGB(0, 0, 255), 2.0);
	PicoLed::Particles effectParticles(ledStrip, firePalette, 0.5, 1.5);
	effectParticles.addSource(16, 0.75, -0.3);
	effectParticles.addSource(17, 0.75, 0.3);

	uint effectIndex = 0;
	uint effectCount = 5;
	bool effectReset = false;
	unsigned int effectChangeInterval = 20000; // Change effect every 20 seconds
	unsigned int timeNow = to_ms_since_boot(get_absolute_time());
	unsigned int timeNext = timeNow + effectChangeInterval;

	while (true) {
		switch (effectIndex) {
		case 0:
			if (effectReset) {
				effectReset = false;
				effectParticles.reset();
			}
			if (effectParticles.animate()) {
				ledStrip.show();
			}
			break;
		case 1:
			if (effectReset) {
				effectReset = false;
				effectMarquee.reset();
			}
			if (effectMarquee.animate()) {
				ledStrip.show();
			}
			break;
		case 2:
			if (effectReset) {
				effectReset = false;
				effectStars.reset();
			}
			if (effectStars.animate()) {
				ledStrip.show();
			}
			break;
		case 3:
			if (effectReset) {
				effectReset = false;
				effectComet.reset();
			}
			if (effectComet.animate()) {
				ledStrip.show();
			}
			break;
		case 4:
			if (effectReset) {
				effectReset = false;
				effectBounce.reset();
			}
			if (effectBounce.animate()) {
				ledStrip.show();
			}
			break;
		}
		sleep_ms(5);
		// Check for effect change
		timeNow = to_ms_since_boot(get_absolute_time());
		if (timeNow > timeNext) {
			timeNext = timeNow + effectChangeInterval;
			effectIndex = (effectIndex + 1) % effectCount;
			effectReset = true;
		}
	}
	
	return EXIT_SUCCESS;
}

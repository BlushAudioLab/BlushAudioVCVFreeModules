#include "plugin.hpp"


struct ButtonTest : Module {
	enum ParamId {
		MODE_SWITCH, //my switch to test
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUTS_LEN
	};
	enum LightId {
		BLINK_LIGHT,
		BLINK_LIGHT2,
		BLINK_LIGHT3,
		BLINK_LIGHT4,
		BLINK_LIGHT5,
		BLINK_LIGHT6,
		LIGHTS_LEN
	};

	ButtonTest() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(MODE_SWITCH, 0.0f,5.0f,0.0f,"Mode","",0.0f,1.0f,1.0f);
	}

	void process(const ProcessArgs& args) override {

		float state = params[MODE_SWITCH].getValue();

		if(state == 0.0f){

			lights[BLINK_LIGHT].setBrightness(1.0f);
			lights[BLINK_LIGHT2].setBrightness(0.0f);
			lights[BLINK_LIGHT3].setBrightness(0.0f);
			lights[BLINK_LIGHT4].setBrightness(0.0f);
			lights[BLINK_LIGHT5].setBrightness(0.0f);
			lights[BLINK_LIGHT6].setBrightness(0.0f);

		}
		else if(state == 1.0f){
			lights[BLINK_LIGHT].setBrightness(0.0f);
			lights[BLINK_LIGHT2].setBrightness(1.0f);
			lights[BLINK_LIGHT3].setBrightness(0.0f);
			lights[BLINK_LIGHT4].setBrightness(0.0f);
			lights[BLINK_LIGHT5].setBrightness(0.0f);
			lights[BLINK_LIGHT6].setBrightness(0.0f);
		}
		else if(state == 2.0f){
			lights[BLINK_LIGHT].setBrightness(0.0f);
			lights[BLINK_LIGHT2].setBrightness(0.0f);
			lights[BLINK_LIGHT3].setBrightness(1.0f);
			lights[BLINK_LIGHT4].setBrightness(0.0f);
			lights[BLINK_LIGHT5].setBrightness(0.0f);
			lights[BLINK_LIGHT6].setBrightness(0.0f);
		}
		else if(state == 3.0f){
			lights[BLINK_LIGHT].setBrightness(0.0f);
			lights[BLINK_LIGHT2].setBrightness(0.0f);
			lights[BLINK_LIGHT3].setBrightness(0.0f);
			lights[BLINK_LIGHT4].setBrightness(1.0f);
			lights[BLINK_LIGHT5].setBrightness(0.0f);
			lights[BLINK_LIGHT6].setBrightness(0.0f);
		}
		else if(state == 4.0f){
			lights[BLINK_LIGHT].setBrightness(0.0f);
			lights[BLINK_LIGHT2].setBrightness(0.0f);
			lights[BLINK_LIGHT3].setBrightness(0.0f);
			lights[BLINK_LIGHT4].setBrightness(0.0f);
			lights[BLINK_LIGHT5].setBrightness(1.0f);
			lights[BLINK_LIGHT6].setBrightness(0.0f);
		}
		else if(state == 5.0f){
			lights[BLINK_LIGHT].setBrightness(0.0f);
			lights[BLINK_LIGHT2].setBrightness(0.0f);
			lights[BLINK_LIGHT3].setBrightness(0.0f);
			lights[BLINK_LIGHT4].setBrightness(0.0f);
			lights[BLINK_LIGHT5].setBrightness(0.0f);
			lights[BLINK_LIGHT6].setBrightness(1.0f);
		}
	}
};

struct OnOff : app::SvgSwitch {
	OnOff() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing3.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing4.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing5.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/drawing6.svg")));		
	}
};

struct ButtonTestWidget : ModuleWidget {
	ButtonTestWidget(ButtonTest* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ButtonTest.svg")));

		// addChild(createParamCentered<OnOff>(mm2px(Vec(15.24, 23.469)), module, ButtonTest::MODE_SWITCH));
		addParam(createParamCentered<OnOff>(mm2px(Vec(15.24, 23.469)), module, ButtonTest::MODE_SWITCH));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 37.324)), module, ButtonTest::BLINK_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 50.615)), module, ButtonTest::BLINK_LIGHT2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 63.907)), module, ButtonTest::BLINK_LIGHT3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 77.199)), module, ButtonTest::BLINK_LIGHT4));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 90.49)), module, ButtonTest::BLINK_LIGHT5));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 103.782)), module, ButtonTest::BLINK_LIGHT6));
		
	}
};

Model* modelButtonTest = createModel<ButtonTest, ButtonTestWidget>("ButtonTest");
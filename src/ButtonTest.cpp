#include "plugin.hpp"


struct ButtonTest : Module {
	enum ParamId {
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

	// struct sub_sw_2 : SVGSwitch, ToggleSwitch {
	// 	sub_sw_2() {
	// 		addFrame(SVG::load(assetPlugin(plugin, "res/Components/sub_sw_2a.svg")));
	// 		addFrame(SVG::load(assetPlugin(plugin, "res/Components/sub_sw_2b.svg")));
	// 	}
	// };

	ButtonTest() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
	}

	void process(const ProcessArgs& args) override {
	}
};


struct ButtonTestWidget : ModuleWidget {
	ButtonTestWidget(ButtonTest* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ButtonTest.svg")));

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

		addChild(createWidgetCentered<Widget>(mm2px(Vec(15.24, 23.469))));
	}
};


Model* modelButtonTest = createModel<ButtonTest, ButtonTestWidget>("ButtonTest");
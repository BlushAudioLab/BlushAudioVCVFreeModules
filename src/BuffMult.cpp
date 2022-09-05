#include "plugin.hpp"


struct BuffMult : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		AUDIOCVIN1_INPUT,
		AUDIOCVIN2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		AUDIOCVOUT1_OUTPUT,
		AUDIOCVOUT2_OUTPUT,
		AUDIOCVOUT3_OUTPUT,
		AUDIOCVOUT4_OUTPUT,
		AUDIOCVOUT5_OUTPUT,
		AUDIOCVOUT6_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	BuffMult() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(AUDIOCVIN1_INPUT, "");
		configInput(AUDIOCVIN2_INPUT, "");
		configOutput(AUDIOCVOUT1_OUTPUT, "");
		configOutput(AUDIOCVOUT2_OUTPUT, "");
		configOutput(AUDIOCVOUT3_OUTPUT, "");
		configOutput(AUDIOCVOUT4_OUTPUT, "");
		configOutput(AUDIOCVOUT5_OUTPUT, "");
		configOutput(AUDIOCVOUT6_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct BuffMultWidget : ModuleWidget {
	BuffMultWidget(BuffMult* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/BuffMult.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 21.386)), module, BuffMult::AUDIOCVIN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 69.261)), module, BuffMult::AUDIOCVIN2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 33.355)), module, BuffMult::AUDIOCVOUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 45.324)), module, BuffMult::AUDIOCVOUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 57.292)), module, BuffMult::AUDIOCVOUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 81.23)), module, BuffMult::AUDIOCVOUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 93.199)), module, BuffMult::AUDIOCVOUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 105.167)), module, BuffMult::AUDIOCVOUT6_OUTPUT));
	}
};


Model* modelBuffMult = createModel<BuffMult, BuffMultWidget>("BuffMult");
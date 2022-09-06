#include "plugin.hpp"


struct BuffMix : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		AUDIOCVIN1_INPUT,
		AUDIOCVOUT1_INPUT,
		AUDIOCVOUT2_INPUT,
		AUDIOCVIN2_INPUT,
		AUDIOCVOUT4_INPUT,
		AUDIOCVOUT5_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		AUDIOCVOUT3_OUTPUT,
		AUDIOCVOUT6_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	BuffMix() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(AUDIOCVIN1_INPUT, "");
		configInput(AUDIOCVOUT1_INPUT, "");
		configInput(AUDIOCVOUT2_INPUT, "");
		configInput(AUDIOCVIN2_INPUT, "");
		configInput(AUDIOCVOUT4_INPUT, "");
		configInput(AUDIOCVOUT5_INPUT, "");
		configOutput(AUDIOCVOUT3_OUTPUT, "");
		configOutput(AUDIOCVOUT6_OUTPUT, "");
	}
	
struct BuffMixWidget : ModuleWidget {
	BuffMixWidget(BuffMix* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/BuffMix.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 18.74)), module, BuffMix::AUDIOCVIN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 31.465)), module, BuffMix::AUDIOCVOUT1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 44.19)), module, BuffMix::AUDIOCVOUT2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 69.639)), module, BuffMix::AUDIOCVIN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 82.364)), module, BuffMix::AUDIOCVOUT4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 95.089)), module, BuffMix::AUDIOCVOUT5_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 56.914)), module, BuffMix::AUDIOCVOUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 107.813)), module, BuffMix::AUDIOCVOUT6_OUTPUT));
	}
};


Model* modelBuffMix = createModel<BuffMix, BuffMixWidget>("BuffMix");
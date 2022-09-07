#include "plugin.hpp"


struct BuffMix : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		//clever way of setting input numbers - used in for loop later
		ENUMS(IN_INPUTS, 6),
		INPUTS_LEN
	};
	enum OutputId {
		//clever way of setting input numbers - used in for loop later
		ENUMS(OUT_OUTPUTS, 2),
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	BuffMix() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		//for loop that counts through number of inputs and appends the number
		for (int i = 0; i < 6; i++)
			configInput(IN_INPUTS + i, string::f("Input %d", i + 1));
		//for loop that counts through number of outputs and appends the number
		for (int i = 0; i < 2; i++)
			configInput(OUT_OUTPUTS + i, string::f("Mix %d", i + 1));
	}

	void process(const ProcessArgs& args) override {
	


		

	}
};	
	
struct BuffMixWidget : ModuleWidget {
	BuffMixWidget(BuffMix* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/BuffMix.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 18.74)), module, BuffMix::IN_INPUTS + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 31.465)), module, BuffMix::IN_INPUTS + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 44.19)), module, BuffMix::IN_INPUTS + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 69.639)), module, BuffMix::IN_INPUTS + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 82.364)), module, BuffMix::IN_INPUTS + 4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 95.089)), module, BuffMix::IN_INPUTS + 5));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 56.914)), module, BuffMix::OUT_OUTPUTS + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 107.813)), module, BuffMix::OUT_OUTPUTS + 1));
	}
};

Model* modelBuffMix = createModel<BuffMix, BuffMixWidget>("BuffMix");
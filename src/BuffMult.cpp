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

		//setting up the input
		int n = inputs[AUDIOCVIN1_INPUT].getChannels();
		float* out = inputs[AUDIOCVIN1_INPUT].getVoltages();

		//routing input to outputs 1,2,3
		outputs[AUDIOCVOUT1_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT1_OUTPUT].writeVoltages(out);
		outputs[AUDIOCVOUT2_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT2_OUTPUT].writeVoltages(out);
		outputs[AUDIOCVOUT3_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT3_OUTPUT].writeVoltages(out);

		//statement taking care of "normalling" behaviour for all outputs below this - i.e 4,5,6
		if (inputs[AUDIOCVIN2_INPUT].isConnected()){
			n = inputs[AUDIOCVIN2_INPUT].getChannels();
			out = inputs[AUDIOCVIN2_INPUT].getVoltages();	
		}

		//routing input to ouputs 4,5,6 "normalled" behaviour from Input A when Input B not connected, using Input B when connected
		outputs[AUDIOCVOUT4_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT4_OUTPUT].writeVoltages(out);
		outputs[AUDIOCVOUT5_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT5_OUTPUT].writeVoltages(out);
		outputs[AUDIOCVOUT6_OUTPUT].setChannels(n);
		outputs[AUDIOCVOUT6_OUTPUT].writeVoltages(out);
		
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

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 18.73)), module, BuffMult::AUDIOCVIN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 69.6)), module, BuffMult::AUDIOCVIN2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 31.5)), module, BuffMult::AUDIOCVOUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 44.1)), module, BuffMult::AUDIOCVOUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 56.9)), module, BuffMult::AUDIOCVOUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 82.3)), module, BuffMult::AUDIOCVOUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 95)), module, BuffMult::AUDIOCVOUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 107.8)), module, BuffMult::AUDIOCVOUT6_OUTPUT));
	}
};


Model* modelBuffMult = createModel<BuffMult, BuffMultWidget>("BuffMult");
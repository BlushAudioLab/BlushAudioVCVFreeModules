#include "plugin.hpp"

struct BuffMult : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		IN1_INPUT,
		IN2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		OUT5_OUTPUT,
		OUT6_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	BuffMult() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(IN1_INPUT, "Input 1");
		configInput(IN2_INPUT, "Input 2");
		configOutput(OUT1_OUTPUT, "Output 1");
		configOutput(OUT2_OUTPUT, "Output 2");
		configOutput(OUT3_OUTPUT, "Output 3");
		configOutput(OUT4_OUTPUT, "Output 4");
		configOutput(OUT5_OUTPUT, "Output 5");
		configOutput(OUT6_OUTPUT, "Output 6");
	}

	void process(const ProcessArgs& args) override {

		//setting up the input
		int in = inputs[IN1_INPUT].getChannels();
		float* out = inputs[IN1_INPUT].getVoltages();

		//routing input to outputs 1,2,3
		outputs[OUT1_OUTPUT].setChannels(in);
		outputs[OUT1_OUTPUT].writeVoltages(out);
		outputs[OUT2_OUTPUT].setChannels(in);
		outputs[OUT2_OUTPUT].writeVoltages(out);
		outputs[OUT3_OUTPUT].setChannels(in);
		outputs[OUT3_OUTPUT].writeVoltages(out);

		//statement taking care of "normalling" behaviour for all outputs below this - i.e 4,5,6
		if (inputs[IN2_INPUT].isConnected()){
			in = inputs[IN2_INPUT].getChannels();
			out = inputs[IN2_INPUT].getVoltages();	
		}

		//routing input to ouputs 4,5,6 "normalled" behaviour from Input 1 when Input 2 not connected, using Input 2 when connected
		outputs[OUT4_OUTPUT].setChannels(in);
		outputs[OUT4_OUTPUT].writeVoltages(out);
		outputs[OUT5_OUTPUT].setChannels(in);
		outputs[OUT5_OUTPUT].writeVoltages(out);
		outputs[OUT6_OUTPUT].setChannels(in);
		outputs[OUT6_OUTPUT].writeVoltages(out);
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

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 18.73)), module, BuffMult::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 69.6)), module, BuffMult::IN2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 31.5)), module, BuffMult::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 44.1)), module, BuffMult::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 56.9)), module, BuffMult::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 82.3)), module, BuffMult::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 95)), module, BuffMult::OUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 107.8)), module, BuffMult::OUT6_OUTPUT));
	}
};

Model* modelBuffMult = createModel<BuffMult, BuffMultWidget>("BuffMult");
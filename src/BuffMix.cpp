#include "plugin.hpp"

using simd::float_4;

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
		//less clever way of setting output numbers
		OUT1_OUTPUT,
		OUT2_OUTPUT,
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
		configOutput(OUT1_OUTPUT, "Output 1");
		configOutput(OUT2_OUTPUT, "Output 2");
	}
	

	void process(const ProcessArgs& args) override {
	
		// number of channels and connected inputs
		int channels = 1;
		int connected = 0;
		for (int i = 0; i < 6; i++){
			channels = std::max(channels, inputs[IN_INPUTS + i].getChannels());
			if (inputs[IN_INPUTS + i].isConnected())
				connected++;
		}
		// set the gain to automatically be divided by the number of connected inputs - unity gain by default
		float gain = 1.f;
		gain /= std::max(1, connected);

	
		for (int ch = 0; ch < channels; ch += 4){
			float_4 out = 0.f;
			float_4 out2 = 0.f;
		
			//mix the inputs
			if (outputs[OUT1_OUTPUT].isConnected()){ // if output 1 is connected route inputs 1-3 to output 1 and 4-6 to output 2
		
				for (int i = 0; i < 3; i++){
					out += inputs[IN_INPUTS + i].getVoltageSimd<float_4>(ch);
					out2 += inputs[IN_INPUTS + (i + 3)].getVoltageSimd<float_4>(ch);
				}
				// applying the gain
				out *= gain;
				out2 *= gain; 		
				// set outputs
				outputs[OUT1_OUTPUT].setVoltageSimd(out, ch);
				outputs[OUT2_OUTPUT].setVoltageSimd(out2, ch);
			}
			else{ // route all 6 inputs to output 2	
				
				for (int i = 0; i < 6; i++){
					out += inputs[IN_INPUTS + i].getVoltageSimd<float_4>(ch);
				}
				// applying the gain
				out *= gain; 
				// set outputs
				outputs[OUT2_OUTPUT].setVoltageSimd(out, ch);
			}			
		}
		//set the channels of each output
		outputs[OUT1_OUTPUT].setChannels(channels);
		outputs[OUT2_OUTPUT].setChannels(channels);		
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

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 56.914)), module, BuffMix::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 107.813)), module, BuffMix::OUT2_OUTPUT));
	}
};

Model* modelBuffMix = createModel<BuffMix, BuffMixWidget>("BuffMix");
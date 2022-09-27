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

		if (((outputs[OUT1_OUTPUT].isConnected() && (!(outputs[OUT2_OUTPUT].isConnected()))))){
			for (int i = 0; i < 3; i++){
				channels = std::max(channels, inputs[IN_INPUTS + i].getChannels());
				if (inputs[IN_INPUTS + i].isConnected())
					connected++;
			}
		}
		else if (((outputs[OUT1_OUTPUT].isConnected() && ((outputs[OUT2_OUTPUT].isConnected()))))){
			for (int i = 0; i < 3; i++){
				channels = std::max(channels, inputs[IN_INPUTS + (i + 3)].getChannels());
				if (inputs[IN_INPUTS + i].isConnected())
					connected++;
			}
		}
		else{
			for (int i = 0; i < 6; i++){
				channels = std::max(channels, inputs[IN_INPUTS + i].getChannels());
				if (inputs[IN_INPUTS + i].isConnected())
					connected++;
			}
		}
		// set the gain to automatically be divided by the number of connected inputs - unity gain by default
		float gain1 = 1.f;
		gain1 /= std::max(1, connected);
	
		for (int ch = 0; ch < channels; ch += 4){
			float_4 out = 0.f;
			float_4 out2 = 0.f;

			//if only the 1st output is connected route 1-3 to output 1, and ignore 4-6
			if ((outputs[OUT1_OUTPUT].isConnected() && (!(outputs[OUT2_OUTPUT].isConnected())))){ 	
				
				for (int i = 0; i < 3; i++){
					out += inputs[IN_INPUTS + i].getVoltageSimd<float_4>(ch);

				}

				// applying the gain
				out *= gain1; 
				out2 *= 0; 		
				// set outputs
				outputs[OUT1_OUTPUT].setVoltageSimd(out, ch);
				outputs[OUT2_OUTPUT].setVoltageSimd(out2, ch);
			}	

			//if only the 2nd ouputs is connected route all inputs to output 2
			else if (!(outputs[OUT1_OUTPUT].isConnected() && ((outputs[OUT2_OUTPUT].isConnected())))){ 	
				
				for (int i = 0; i < 6; i++){
					out2 += inputs[IN_INPUTS + i].getVoltageSimd<float_4>(ch);
				}

				// applying the gain
				out *= 0; 
				out2 *= gain1; 		
				// set outputs
				outputs[OUT1_OUTPUT].setVoltageSimd(out, ch);
				outputs[OUT2_OUTPUT].setVoltageSimd(out2, ch);
			}

			//if both outputs are connected route 1-3 to output 1, are 3-6 to output 2
			else if ((outputs[OUT1_OUTPUT].isConnected() && ((outputs[OUT2_OUTPUT].isConnected())))){ 	
				
				for (int i = 0; i < 3; i++){
					out += inputs[IN_INPUTS + i].getVoltageSimd<float_4>(ch);
					out2 += inputs[IN_INPUTS + (i + 3)].getVoltageSimd<float_4>(ch);
				}

				// applying the gain
				out *= gain1; 
				out2 *= gain1; 		
				// set outputs
				outputs[OUT1_OUTPUT].setVoltageSimd(out, ch);
				outputs[OUT2_OUTPUT].setVoltageSimd(out2, ch);
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

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		// addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 18.74)), module, BuffMix::IN_INPUTS + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 31.465)), module, BuffMix::IN_INPUTS + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 44.19)), module, BuffMix::IN_INPUTS + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 69.639)), module, BuffMix::IN_INPUTS + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 82.364)), module, BuffMix::IN_INPUTS + 4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, 95.089)), module, BuffMix::IN_INPUTS + 5));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, 56.914)), module, BuffMix::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, 107.813)), module, BuffMix::OUT2_OUTPUT));
	}
};

Model* modelBuffMix = createModel<BuffMix, BuffMixWidget>("BuffMix");
#include "plugin.hpp"
#include "TrackAndField.hpp"

struct TrackAndField : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		SAMPLE_INPUT,
		TRIGGER_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		TF_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		STATUS_LIGHT,
		LIGHTS_LEN
	};

	TrackAndField() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(SAMPLE_INPUT, "Sample");
		configInput(TRIGGER_INPUT, "Trigger");
		configLight(STATUS_LIGHT, "Status");
		configOutput(TF_OUTPUT, "Output");
	}

	void process(const ProcessArgs& args) override {

		const float gain = 5.f / std::sqrt(2.f);
		white = random::normal();

		sampleInput = inputs[SAMPLE_INPUT].getVoltage();
		triggerInput = inputs[TRIGGER_INPUT].getVoltage();

		if(!(inputs[SAMPLE_INPUT].isConnected()) && (triggerInput > 0.f)){
			
			outputs[TF_OUTPUT].setVoltage(white * gain);
			lights[STATUS_LIGHT].setBrightness(white * gain);

		}
		else{

			outputs[TF_OUTPUT].setVoltage(0);
			lights[STATUS_LIGHT].setBrightness(0);

		}

		





	}
};

struct TrackAndFieldWidget : ModuleWidget {
	TrackAndFieldWidget(TrackAndField* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/TrackAndField.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 21.269)), module, TrackAndField::SAMPLE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 21.269)), module, TrackAndField::TRIGGER_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 38.794)), module, TrackAndField::TF_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 38.794)), module, TrackAndField::STATUS_LIGHT));
	}
};


Model* modelTrackAndField = createModel<TrackAndField, TrackAndFieldWidget>("TrackAndField");
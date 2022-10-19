#include "plugin.hpp"
#include "Coloratura.hpp"
#include <samplerate.h>

struct Coloratura : Module {
	enum ParamId {
		DELAY_PARAM, //delay time
		FEEDBACK_PARAM, //feedback amount
		MIX_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		CHORUS_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		CHORUS_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	//constructor
	Coloratura() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		const float delayMin = log10(0.001f * 1000) / 4;
		const float delayMax = log10(10.f * 1000) / 4;
		const float delayDefault = log10(0.5f * 1000) / 4;

		configParam(DELAY_PARAM, delayMin, delayMax, delayDefault, "Delay", " s", 10.f / 1e-3, 1e-3);
		configParam(FEEDBACK_PARAM, 0.f, 1.f, 0.5f, "Feedback", "%", 0, 100 );
		configParam(MIX_PARAM, 0.0f, 1.0f, 0.5f, "Mix", "%", 0.0f, 100.0f);

		configInput(CHORUS_INPUT, "Input");
		configOutput(CHORUS_OUTPUT, "Output");

		// src = src_new(SRC_SINC_FASTEST, 1, NULL);
	}

	//destructor
	~Coloratura() {
		// src_delete(src); //probably deleting some memory?
	}

	void process(const ProcessArgs& args) override {

		clockFreq = 2.f;
		float in = inputs[CHORUS_INPUT].getVoltageSum();
	    float feedback = 1.f;
		outputs[CHORUS_OUTPUT].setVoltage(in + lastWet * feedback);

	}
};

struct ColoraturaWidget : ModuleWidget {
	ColoraturaWidget(Coloratura* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Coloratura.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 103.586)), module, Coloratura::CHORUS_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.182, 103.586)), module, Coloratura::CHORUS_OUTPUT));
	}
};

Model* modelColoratura = createModel<Coloratura, ColoraturaWidget>("Coloratura");
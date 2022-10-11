#include "plugin.hpp"

struct Coloratura : Module {
	enum ParamId {
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

	Coloratura() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(CHORUS_INPUT, "Input");
		configOutput(CHORUS_OUTPUT, "Output");
	}

	void process(const ProcessArgs& args) override {
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
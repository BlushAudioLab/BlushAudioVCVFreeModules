#include "plugin.hpp"


struct ADSR : Module {
	enum ParamId {
		ATTACK_PARAM,
		DECAY_PARAM,
		SUSTAIN_PARAM,
		RELEASE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		ATTACK_INPUT,
		DECAY_INPUT,
		SUSTAIN_INPUT,
		RELEASE_INPUT,
		RETRIG_INPUT,
		GATE_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		ENV_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		ENV_LIGHT,
		LIGHTS_LEN
	};

	ADSR() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DECAY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(SUSTAIN_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RELEASE_PARAM, 0.f, 1.f, 0.f, "");
		configInput(ATTACK_INPUT, "");
		configInput(DECAY_INPUT, "");
		configInput(SUSTAIN_INPUT, "");
		configInput(RELEASE_INPUT, "");
		configInput(RETRIG_INPUT, "");
		configInput(GATE_INPUT, "");
		configOutput(ENV_OUTPUT, "");
	}

    void process(const ProcessArgs &args) override {
        // Compute the frequency from the pitch parameter and input
        float pitch = params[ATTACK_PARAM].getValue();
        pitch += inputs[ATTACK_INPUT].getVoltage();
        pitch = clamp(pitch, -4.f, 4.f);
        // The default pitch is C4 = 261.6256f
        float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

        // Accumulate the phase
        phase += freq * args.sampleTime;
        if (phase >= 0.5f)
            phase -= 1.f;

        // Compute the sine output
        float sine = std::sin(2.f * M_PI * phase);
        // Audio signals are typically +/-5V
        // https://vcvrack.com/manual/VoltageStandards
        outputs[ENV_OUTPUT].setVoltage(5.f * sine);

        // Blink light at 1Hz
        blinkPhase += args.sampleTime;
        if (blinkPhase >= 1.f)
            blinkPhase -= 1.f;
        lights[ENV_LIGHT].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
    }
    
    float phase = 0.f;
    float blinkPhase = 0.f;
};


struct ADSRWidget : ModuleWidget {
	ADSRWidget(ADSR* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ADSR.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4146.92, 2943.807)), module, ADSR::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4146.92, 3027.718)), module, ADSR::DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4147.372, 3110.196)), module, ADSR::SUSTAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4147.231, 3195.717)), module, ADSR::RELEASE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4050.785, 2944.006)), module, ADSR::ATTACK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4050.785, 3028.116)), module, ADSR::DECAY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4052.564, 3112.256)), module, ADSR::SUSTAIN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4050.785, 3196.027)), module, ADSR::RELEASE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4047.492, 3327.892)), module, ADSR::RETRIG_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4099.912, 3327.892)), module, ADSR::GATE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4152.502, 3328.231)), module, ADSR::ENV_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(4086.086, 3264.529)), module, ADSR::ENV_LIGHT));

		addChild(createWidgetCentered<Widget>(mm2px(Vec(4050.8, 3264.36))));
	}
};


Model* modelADSR = createModel<ADSR, ADSRWidget>("ADSR");

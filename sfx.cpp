///////////////////////////////////
// sfxr - sound effect generator //
//    by DrPetter, 2007-12-14    //
//     developed for LD48#10     //
// drpetter.se/project_sfxr.html //
///////////////////////////////////

// Sfx
struct Sfx {
private:
	int wave_type;

	float p_base_freq;
	float p_freq_limit;
	float p_freq_ramp;
	float p_freq_dramp;
	float p_duty;
	float p_duty_ramp;

	float p_vib_strength;
	float p_vib_speed;
	float p_vib_delay;

	float p_env_attack;
	float p_env_sustain;
	float p_env_decay;
	float p_env_punch;

	bool filter_on;
	float p_lpf_resonance;
	float p_lpf_freq;
	float p_lpf_ramp;
	float p_hpf_freq;
	float p_hpf_ramp;

	float p_pha_offset;
	float p_pha_ramp;

	float p_repeat_speed;

	float p_arp_speed;
	float p_arp_mod;

	float master_vol;

	float sound_vol;

	bool playing_sample;
	int phase;
	double fperiod;
	double fmaxperiod;
	double fslide;
	double fdslide;
	int period;
	float square_duty;
	float square_slide;
	int env_stage;
	int env_time;
	int env_length[3];
	float env_vol;
	float fphase;
	float fdphase;
	int iphase;
	float phaser_buffer[1024];
	int ipp;
	float noise_buffer[32];
	float fltp;
	float fltdp;
	float fltw;
	float fltw_d;
	float fltdmp;
	float fltphp;
	float flthp;
	float flthp_d;
	float vib_phase;
	float vib_speed;
	float vib_amp;
	int rep_time;
	int rep_limit;
	int arp_time;
	int arp_limit;
	double arp_mod;

	float* vselected;
	int vcurbutton;

	int wav_bits;
	int wav_freq;

	int   file_sampleswritten;
	float filesample;
	int   fileacc;

	void ResetSample(bool restart) {
		if (!restart)
			phase = 0;

		fperiod = 100.0 / (p_base_freq * p_base_freq + 0.001);
		period = (int)fperiod;
		fmaxperiod = 100.0 / (p_freq_limit * p_freq_limit + 0.001);
		fslide = 1.0 - pow((double)p_freq_ramp, 3.0) * 0.01;
		fdslide = -pow((double)p_freq_dramp, 3.0) * 0.000001;
		square_duty = 0.5 - p_duty * 0.5;
		square_slide = -p_duty_ramp * 0.00005;

		if (p_arp_mod >= 0.0)
			arp_mod = 1.0 - pow((double)p_arp_mod, 2.0) * 0.9;
		else
			arp_mod = 1.0 + pow((double)p_arp_mod, 2.0) * 10.0;

		arp_time = 0;
		arp_limit = (int)(pow(1 - p_arp_speed, 2) * 20000 + 32);
		if (p_arp_speed == 1)
			arp_limit = 0;

		if (!restart) {
			// reset filter
			fltp = 0;
			fltdp = 0;
			fltw = pow(p_lpf_freq, 3) * 0.1;
			fltw_d = 1 + p_lpf_ramp * 0.0001;
			fltdmp = 5 / (1 + pow(p_lpf_resonance, 2) * 20) * (0.01 + fltw);
			if (fltdmp > 0.8) fltdmp = 0.8;
			fltphp = 0;
			flthp = pow(p_hpf_freq, 2) * 0.1;
			flthp_d = 1.0 + p_hpf_ramp * 0.0003;

			// reset vibrato
			vib_phase = 0;
			vib_speed = pow(p_vib_speed, 2) * 0.01;
			vib_amp = p_vib_strength * 0.5;

			// reset envelope
			env_vol = 0;
			env_stage = 0;
			env_time = 0;
			env_length[0] = (int)(p_env_attack * p_env_attack * 100000);
			env_length[1] = (int)(p_env_sustain * p_env_sustain * 100000);
			env_length[2] = (int)(p_env_decay * p_env_decay * 100000);

			fphase = pow(p_pha_offset, 2) * 1020;
			if (p_pha_offset < 0) fphase = -fphase;
			fdphase = pow(p_pha_ramp, 2);
			if (p_pha_ramp < 0) fdphase = -fdphase;
			iphase = abs((int)fphase);
			ipp = 0;

			for (int i = 0; i < 1024; i++)
				phaser_buffer[i] = 0;

			for (int i = 0; i < 32; i++)
				noise_buffer[i] = (rand() % 10001) / 10000.0 * 2 - 1;

			rep_time = 0;
			rep_limit = (int)(pow(1 - p_repeat_speed, 2) * 20000 + 32);
			if (p_repeat_speed == 0)
				rep_limit = 0;
		}
	}

	void PlaySample() {
		ResetSample(false);
		playing_sample = true;
	}

	void SynthSample(int length, float * buffer, FILE * file) {
		for (int i = 0; i < length; i++) {
			if (!playing_sample)
				break;

			rep_time++;
			if (rep_limit != 0 && rep_time >= rep_limit) {
				rep_time = 0;
				ResetSample(true);
			}

			// frequency envelopes/arpeggios
			arp_time++;
			if (arp_limit != 0 && arp_time >= arp_limit) {
				arp_limit = 0;
				fperiod *= arp_mod;
			}
			fslide += fdslide;
			fperiod *= fslide;
			if (fperiod > fmaxperiod) {
				fperiod = fmaxperiod;
				if (p_freq_limit > 0)
					playing_sample = false;
			}
			float rfperiod = fperiod;
			if (vib_amp > 0) {
				vib_phase += vib_speed;
				rfperiod = fperiod * (1.0 + sin(vib_phase) * vib_amp);
			}
			period = (int)rfperiod;
			if (period < 8) period = 8;
			square_duty += square_slide;
			if (square_duty < 0) square_duty = 0;
			if (square_duty > 0.5) square_duty = 0.5;

			// volume envelope
			env_time++;
			if (env_time > env_length[env_stage]) {
				env_time = 0;
				env_stage++;
				if (env_stage == 3)
					playing_sample = false;
			}
			if (env_stage == 0)
				env_vol = (float)env_time / env_length[0];
			if (env_stage == 1)
				env_vol = 1 + pow(1 - (float)env_time / env_length[1], 1) * 2 * p_env_punch;
			if (env_stage == 2)
				env_vol = 1 - (float)env_time / env_length[2];

			// phaser step
			fphase += fdphase;
			iphase = abs((int)fphase);
			if (iphase > 1023) iphase = 1023;

			if (flthp_d != 0) {
				flthp *= flthp_d;
				if (flthp < 0.00001) flthp = 0.00001;
				if (flthp > 0.1) flthp = 0.1;
			}

			// 8x supersampling
			float ssample = 0;
			for (int si = 0; si < 8; si++) {
				float sample = 0;
				phase++;
				if (phase >= period) {
					// phase = 0;
					phase %= period;
					if (wave_type == 3)
						for (int i = 0; i < 32; i++)
							noise_buffer[i] = (rand() % 10001) / 10000.0 * 2 - 1;
				}

				// base waveform
				float fp = (float)phase / period;
				switch (wave_type) {
				case 0: // square
					if (fp < square_duty)
						sample = 0.5;
					else
						sample = -0.5;
					break;
				case 1: // sawtooth
					sample = 1 - fp * 2;
					break;
				case 2: // sine
					sample = (float)sin(fp * 2 * PI);
					break;
				case 3: // noise
					sample = noise_buffer[phase * 32 / period];
					break;
				}

				// lp filter
				float pp = fltp;
				fltw *= fltw_d;
				if (fltw < 0) fltw = 0;
				if (fltw > 0.1f) fltw = 0.1;
				if (p_lpf_freq != 1) {
					fltdp += (sample - fltp) * fltw;
					fltdp -= fltdp * fltdmp;
				}
				else {
					fltp = sample;
					fltdp = 0;
				}
				fltp += fltdp;

				// hp filter
				fltphp += fltp - pp;
				fltphp -= fltphp * flthp;
				sample = fltphp;

				// phaser
				phaser_buffer[ipp & 1023] = sample;
				sample += phaser_buffer[(ipp - iphase + 1024) & 1023];
				ipp = (ipp + 1) & 1023;

				// final accumulation and envelope application
				ssample += sample * env_vol;
			}
			ssample = ssample / 8 * master_vol;

			ssample *= 2 * sound_vol;

			if (buffer != nullptr) {
				if (ssample > 1) ssample = 1;
				if (ssample < -1) ssample = -1;
				 *buffer++ = ssample;
			}

			if (file != nullptr) {
				// quantize depending on format
				// accumulate/count to accomodate variable sample rate?
				ssample *= 4; // arbitrary gain to get reasonable output volume...
				if (ssample > 1) ssample = 1;
				if (ssample < -1) ssample = -1;
				filesample += ssample;
				fileacc++;
				if (wav_freq == 44100 || fileacc == 2) {
					filesample /= fileacc;
					fileacc = 0;
					if (wav_bits == 16) {
						short isample = (short)(filesample * 32000);
						fwrite(&isample, 1, 2, file);
					}
					else {
						uchar isample = (uchar)(filesample * 127 + 128);
						fwrite(&isample, 1, 1, file);
					}
					filesample = 0;
				}
				file_sampleswritten++;
			}
		}
	}

	bool mute_stream;

	int ExportWAV(char * filename) {
		FILE* foutput;
		fopen_s(&foutput, filename, "wb");
		if (!foutput)
			return 0;

		// write wav header
		uint dword = 0;
		ushort word = 0;
		fwrite("RIFF", 4, 1, foutput); // "RIFF"
		dword = 0;
		fwrite(&dword, 1, 4, foutput); // remaining file size
		fwrite("WAVE", 4, 1, foutput); // "WAVE"

		fwrite("fmt ", 4, 1, foutput); // "fmt"
		dword = 16;
		fwrite(&dword, 1, 4, foutput); // chunk size
		word = 1;
		fwrite(&word, 1, 2, foutput); // compression code
		word = 1;
		fwrite(&word, 1, 2, foutput); // channels
		dword = wav_freq;
		fwrite(&dword, 1, 4, foutput); // sample rate
		dword = wav_freq * wav_bits / 8;
		fwrite(&dword, 1, 4, foutput); // bytes/sec
		word = wav_bits / 8;
		fwrite(&word, 1, 2, foutput); // block align
		word = wav_bits;
		fwrite(&word, 1, 2, foutput); // bits per sample

		fwrite("data", 4, 1, foutput); // "data"
		dword = 0;
		int foutstream_datasize = ftell(foutput);
		fwrite(&dword, 1, 4, foutput); // chunk size

		// write sample data
		mute_stream = true;
		file_sampleswritten = 0;
		filesample = 0;
		fileacc = 0;
		PlaySample();
		while (playing_sample)
			SynthSample(256, nullptr, foutput);
		mute_stream = false;

		// seek back to header and write size info
		fseek(foutput, 4, SEEK_SET);
		dword = 0;
		dword = foutstream_datasize - 4 + file_sampleswritten * wav_bits / 8;
		fwrite(&dword, 1, 4, foutput); // remaining file size
		fseek(foutput, foutstream_datasize, SEEK_SET);
		dword = file_sampleswritten * wav_bits / 8;
		fwrite(&dword, 1, 4, foutput); // chunk size (data)
		fclose(foutput);

		return dword + 44;
	}

public:
	Sfx() {
		master_vol = 0.05;
		sound_vol = 0.5;
		playing_sample = false;
		vselected = nullptr;
		vcurbutton = -1;
		wav_bits = 16;
		wav_freq = 44100;
		filesample = 0;
		fileacc = 0;
	}

	char *create(
		int   _wave_type,
		uchar _sound_vol,
		uchar _p_base_freq,
		uchar _p_freq_limit,
		uchar _p_freq_ramp,
		uchar _p_freq_dramp,
		uchar _p_duty,
		uchar _p_duty_ramp,
		uchar _p_vib_strength,
		uchar _p_vib_speed,
		uchar _p_vib_delay,
		uchar _p_env_attack,
		uchar _p_env_sustain,
		uchar _p_env_decay,
		uchar _p_env_punch,
		bool  _filter_on,
		uchar _p_lpf_resonance,
		uchar _p_lpf_freq,
		uchar _p_lpf_ramp,
		uchar _p_hpf_freq,
		uchar _p_hpf_ramp,
		uchar _p_pha_offset,
		uchar _p_pha_ramp,
		uchar _p_repeat_speed,
		uchar _p_arp_speed,
		uchar _p_arp_mod) {
		wave_type       = _wave_type;
		sound_vol       = char_to_float(_sound_vol);
		p_base_freq     = char_to_float(_p_base_freq);
		p_freq_limit    = char_to_float(_p_freq_limit);
		p_freq_ramp     = char_to_float(_p_freq_ramp);
		p_freq_dramp    = char_to_float(_p_freq_dramp);
		p_duty          = char_to_float(_p_duty);
		p_duty_ramp     = char_to_float(_p_duty_ramp);
		p_vib_strength  = char_to_float(_p_vib_strength);
		p_vib_speed     = char_to_float(_p_vib_speed);
		p_vib_delay     = char_to_float(_p_vib_delay);
		p_env_attack    = char_to_float(_p_env_attack);
		p_env_sustain   = char_to_float(_p_env_sustain);
		p_env_decay     = char_to_float(_p_env_decay);
		p_env_punch     = char_to_float(_p_env_punch);
		filter_on       = _filter_on;
		p_lpf_resonance = char_to_float(_p_lpf_resonance);
		p_lpf_freq      = char_to_float(_p_lpf_freq);
		p_lpf_ramp      = char_to_float(_p_lpf_ramp);
		p_hpf_freq      = char_to_float(_p_hpf_freq);
		p_hpf_ramp      = char_to_float(_p_hpf_ramp);
		p_pha_offset    = char_to_float(_p_pha_offset);
		p_pha_ramp      = char_to_float(_p_pha_ramp);
		p_repeat_speed  = char_to_float(_p_repeat_speed);
		p_arp_speed     = char_to_float(_p_arp_speed);
		p_arp_mod       = char_to_float(_p_arp_mod);

		int size = ExportWAV("sound.wav");
		if (!size) {
			showMessage("sound error");
			return nullptr;
		}

		FILE *file;
		fopen_s(&file, "sound.wav", "rb");
		char *buffer = new char[size];
		fread(buffer, size, 1, file);
		fclose(file);

		remove("sound.wav");
		return buffer;
	}

} sfx;

char* soundJump = sfx.create(
	0,
	float_to_char(0.500000),
	float_to_char(0.408990),
	float_to_char(0.000000),
	float_to_char(0.231340),
	float_to_char(0.000000),
	float_to_char(0.529980),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.107470),
	float_to_char(0.295720),
	float_to_char(0.000000),
	false,
	float_to_char(0.000000),
	float_to_char(1.000000),
	float_to_char(0.000000),
	float_to_char(0.083430),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000),
	float_to_char(0.000000)
);
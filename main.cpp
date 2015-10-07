#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <functional>
#include <exception>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/program_options.hpp>
#include <portaudio.h>
#include <gtk/gtk.h>
#include <pthread.h>
using namespace std;
namespace po = boost::program_options;

/* options:
api:
device:
buffer size: in frames
sample rate:
bit depth:
fragment size: in frames
dither:

*/

/* program options */
int sample_rate;
int bit_depth;
int channels;
int api_id;

/* a frame is, in interleaved pcm audio, a sequence of samples for each channel */
typedef int framecount_t;
framecount_t buffer_size;
framecount_t fragment_size;

typedef int milliseconds_t;
milliseconds_t update_ui_rate;

/* */

int framesize;

std::list<std::string> song_list;

//boost::spsc_queue<char> buffer;

PaStream *stream;

GtkWidget *window;

/* function prototypes */
void init_log(void);
void init_options(void);
void init_gui(void);
void init_portaudio(void);
void enumerate_devices_by_api(int api_idx);

void close_portaudio(void);
void close_log(void);
/* */

int main(int argc, char **argv) {
	init_log();
	gtk_init(&argc, &argv);
	init_options();
	init_gui();
	init_portaudio();

	gtk_main();

	close_portaudio();
	close_log();
	exit(EXIT_SUCCESS);
}

void init_log(void) {
	clog << "\n";
	clog << "ffmpeg version: " << "\n";
	clog << Pa_GetVersionText();
	clog << std::endl;
}

void init_options(void) {
	po::options_description desc("Allowed options");

	desc.add_options()
		("help", "produce help message")
		("sample rate", po::value<int>(), "Set output sample rate")
		("bit depth", po::value<std::string>(), "Set output bit depth")
		("api", po::value<std::string>(), "API to use. It can be alsa, pulseaudio. Empty string uses default.")
		("device", po::value<std::string>(), "Device. It can be alsa, pulseaudio. Empty string uses default.")
;

	if (sample_rate)

}

void init_gui(void) {
	GtkBuilder *builder;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "gui.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show(window);
}

void init_portaudio(void) {
	PaError ret =
	Pa_Initialize();
	if (ret != paNoError) {
		cerr << "Portaudio error: " << Pa_GetErrorText(ret) << "\n";
		exit(EXIT_FAILURE);
	}

	int api_max = Pa_GetHostApiCount();
	if (api_max < 0) { /* if api_max is negative it is actually an error code */
		cerr << "Portaudio error: " << Pa_GetErrorText(api_max) << "\n";
		exit(EXIT_FAILURE);
	}
	if (api_max == 0) {
		cerr << "Error: there is no software for audio output" << "\n";
		exit(EXIT_FAILURE);
	}
	std::vector<const char*> api_names;
	std::vector<PaHostApiTypeId> api_ids;
	for (int api_idx = 0; api_idx < api_max; api_idx++) {
		const PaHostApiInfo* api_info = Pa_GetHostApiInfo(api_idx);
		api_names.push_back(api_info->name);
		api_ids.push_back(api_info->typeId);
	}
	clog << "the system has support for:\n";
	for (int api_idx = 0; api_idx < api_max; api_idx++) {
		clog << " - " << api_names[api_idx] << "\n";
	}
	for (int api_idx = 0; api_idx < api_max; api_idx++) {
		enumerate_devices_by_api(api_idx);
	}

	int api = 0;
	if (api_id == 0) {
		api = Pa_GetDefaultHostApi();
		clog << "using default api '" << Pa_GetHostApiInfo(api)->name << "'\n";
	} else { /* check if requested api is in the 'supported by os' list */
		if (auto std::find(api_typeids.begin(), api_typeids.end(), api) == api_typeids.end()) {
			int old_api_idx =  Pa_HostApiTypeIdToHostApiIndex(api);
			const PaHostApiInfo* api_info = Pa_GetHostApiInfo(old_api);
			if (api_info)
			api = Pa_GetDefaultHostApi();
			clog << "warning: requested api '" << "" << "' is not supported by the system. switched to "
			     << Pa_GetHostApiInfo(api)->name << "\n";
		}
	}
	if (device == 0) {
		device = Pa_GetDefaultOutputDevice();
		if (device == paNoDevice) {
			cerr << "portaudio error: no output device\n"
			exit(EXIT_FAILURE);
		}
	}
	if (sample_rate == 0) {
		sample_rate = (int) Pa_GetDeviceInfo(device)->defaultSampleRate;

	}
/*
	PaStreamParameters outputparam;
	outputparam
	ret =
	Pa_OpenStream(&stream, , , (double) sample_rate, paFramesPerBufferUnspecified, , , NULL);
	if (ret != paNoError) {
		cerr << "Portaudio error: " << Pa_GetErrorText(ret) << "\n";
		exit(EXIT_FAILURE);
	}
	clog << "opened device " << "" << "with following parameters:\n";
	clog << " - sample rate: " << sample_rate << " hz." << "\n";
	clog << " - bit depth: " << bit_depth << " bit." << "\n";
	clog << " - channels: " << channels << "." << "\n";
*/
	clog << std::endl;
}

void enumerate_devices_by_api(int api_idx) {
	const PaHostApiInfo* api_info = Pa_GetHostApiInfo(api_idx);
	clog << "detected devices for " << api_info->name << "\n";

	for (int device_idx_api_relative = 0;
	         device_idx_api_relative < api_info->deviceCount;
	         ++device_idx_api_relative)
	{
		int device_idx_absolute = Pa_HostApiDeviceIndexToDeviceIndex(api_idx, device_idx_api_relative);
		const PaDeviceInfo *device_info = Pa_GetDeviceInfo(device_idx_absolute);
		clog << device_idx_api_relative+1 /* count */ << ": " << device_info->name << "\n";
		clog << "default sample rate: " << device_info->defaultSampleRate << " hz\n";
		clog << "number of output channels: " << device_info->maxOutputChannels << "\n";
	}
}

void process(void* out, framecount_t framecount) {

}

void callback(void) {
}

void background(void) {
}

void async(void) {

}

void update_ui(void) {

	//wait(update_ui_rate);
}

void close_portaudio(void) {
	Pa_Terminate();
}

void close_log(void) {
	clog << "bye!" << std::endl;
}

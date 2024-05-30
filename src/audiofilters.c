#include "audiofilters.h"

#include "xscugic.h"
#include "xscutimer.h"

#include "audioController.h"
#include "defines.h"

#define UPPER_MIDS_LENGTH 	51
const float upperMidsCoefficients[UPPER_MIDS_LENGTH] = {
	0.03171485715822,  0.01665051764101,  0.01619722522076,  0.01242054017441,
   0.006179524277911,-0.0007105809395844,-0.006053611002738,-0.007975518658629,
  -0.005732910812888,-0.0001291766825747, 0.006445281401913,  0.01064626004968,
   0.009123922304965,-0.0001988943079939, -0.01721498925352,  -0.0391427816292,
   -0.06097462269142, -0.07650766930455, -0.08001261688868, -0.06794177283998,
   -0.04008817890798,-0.0002120593620074,  0.04465558413906,  0.08573782543039,
     0.1145289157087,   0.1248738555486,   0.1145289157087,  0.08573782543039,
    0.04465558413906,-0.0002120593620074, -0.04008817890798, -0.06794177283998,
   -0.08001261688868, -0.07650766930455, -0.06097462269142,  -0.0391427816292,
   -0.01721498925352,-0.0001988943079939, 0.009123922304965,  0.01064626004968,
   0.006445281401913,-0.0001291766825747,-0.005732910812888,-0.007975518658629,
  -0.006053611002738,-0.0007105809395844, 0.006179524277911,  0.01242054017441,
    0.01619722522076,  0.01665051764101,  0.03171485715822
};

#define PRESENCE_LENGTH 51
const float presenceCoefficients[PRESENCE_LENGTH] = {
    0.00372019215702136443052472358772320149,
    -0.000492577268638062543311406660251350331,
    0.006765032445048032097578705190699110972,
    0.018139018257523755600635340101689507719,
    0.02106123617285497393036841629054833902,
    0.009155070206171938423933553963252052199,
    -0.011565021091156167665214837825260474347,
    -0.027084252632767896740162427704490255564,
    -0.026878337524249333589931865162725443952,
    -0.012852024511362252975898989859615539899,
    0.002141379657675089413781943648018568638,
    0.005495692500482409793838289857603740529,
    -0.003430419488926570760478629296130748116,
    -0.011498123785303591970485825868308893405,
    -0.002694175661656878634220291246492706705,
    0.026200131930988267148219250657348311506,
    0.058339774570498062478129952523886458948,
    0.06585576970678798380820495594889507629,
    0.030632404808589034500565517760151124094,
    -0.038084015511829660827203269946039654315,
    -0.103837037629437176078361915187997510657,
    -0.123663011014820106714395819835772272199,
    -0.076887546620327207325118479275261051953,
    0.017822021066199008471064857417331950273,
    0.110354863678279110916768956940359203145,
    0.148533163975029774972469454041856806725,
    0.110354863678279110916768956940359203145,
    0.017822021066199008471064857417331950273,
    -0.076887546620327207325118479275261051953,
    -0.123663011014820106714395819835772272199,
    -0.103837037629437176078361915187997510657,
    -0.038084015511829660827203269946039654315,
    0.030632404808589034500565517760151124094,
    0.06585576970678798380820495594889507629,
    0.058339774570498062478129952523886458948,
    0.026200131930988267148219250657348311506,
    -0.002694175661656878634220291246492706705,
    -0.011498123785303591970485825868308893405,
    -0.003430419488926570760478629296130748116,
    0.005495692500482409793838289857603740529,
    0.002141379657675089413781943648018568638,
    -0.012852024511362252975898989859615539899,
    -0.026878337524249333589931865162725443952,
    -0.027084252632767896740162427704490255564,
    -0.011565021091156167665214837825260474347,
    0.009155070206171938423933553963252052199,
    0.02106123617285497393036841629054833902,
    0.018139018257523755600635340101689507719,
    0.006765032445048032097578705190699110972,
    -0.000492577268638062543311406660251350331,
    0.00372019215702136443052472358772320149
};

#define BRILLIANCE_LENGTH 51
const float brillianceCoefficients[BRILLIANCE_LENGTH] = {
    0.01360983433774,-0.001390312836548, -0.02141435929172, -0.01730289178319,
    0.01065168665208,  0.02070789487506, 0.002958579558854,-0.005687400333269,
  0.0007499545829186, -0.01225075406956, -0.03144361549241,-0.002752778261863,
    0.04969300357062,  0.03829936894219, -0.02705250208292, -0.04253784981771,
  -0.001709705875027,-0.001487119806453, -0.02962358024723,  0.02654570047046,
     0.1148413818324,    0.041347725854,  -0.1550363215519,  -0.1702792956594,
    0.07377564400655,   0.2366929872459,  0.07377564400655,  -0.1702792956594,
    -0.1550363215519,    0.041347725854,   0.1148413818324,  0.02654570047046,
   -0.02962358024723,-0.001487119806453,-0.001709705875027, -0.04253784981771,
   -0.02705250208292,  0.03829936894219,  0.04969300357062,-0.002752778261863,
   -0.03144361549241, -0.01225075406956,0.0007499545829186,-0.005687400333269,
   0.002958579558854,  0.02070789487506,  0.01065168665208, -0.01730289178319,
   -0.02141435929172,-0.001390312836548,  0.01360983433774
};

#define AUDIO_BUFFER_SIZE 51
int audioBufferIndex = 0;
audioData audioBuffer[AUDIO_BUFFER_SIZE];

#define FILTER_COUNT 3

void runFilters();
void firFilter(audioData *result, float *coefficients, int filterLength);
void volumeBoost(audioData *audio, float volumeFactor);
void addToAudioBuffer(audioData *audio);
void getFromAudioBuffer(audioData *audio, int decr);

// Handle the interrupt logic
void audioInterruptHandler(void* callbackRef) {
    XScuTimer *timer = (XScuTimer *)callbackRef;

    if (XScuTimer_IsExpired(timer)) {
        XScuTimer_ClearInterruptStatus(timer);
        XScuTimer_LoadTimer(timer, INTERRUPT_PERIOD_US * TIME_TO_US_DIVIDER);
        XScuTimer_Start(timer);

        runFilters();
    }
}

// Main function to run the filters
void runFilters() {
    // Read latest data
    audioData audio;
    readAudio(&audio);

    // Update the audio buffer
    addToAudioBuffer(&audio);


    // Apply the filters
    audioData filterOutputs[FILTER_COUNT];
    firFilter(&filterOutputs[0], upperMidsCoefficients, UPPER_MIDS_LENGTH);
    firFilter(&filterOutputs[1], presenceCoefficients, PRESENCE_LENGTH);
    firFilter(&filterOutputs[2], brillianceCoefficients, BRILLIANCE_LENGTH);

    // Combine the results
    audioData result;
    sumAudio(&result, filterOutputs, FILTER_COUNT);

    // Set volume
    // volumeBoost(&result, 20.0);

    // Write the audio result
    writeAudio(&result);
}

// Apply a FIR filter
void firFilter(audioData *result, float *coefficients, int filterLength) {
    audioData audio;
    getFromAudioBuffer(&audio, 0);

    result->left = 0;
    result->right = 0;

    for (int i = 0; i < filterLength; i++) {
        audioData temp;
        getFromAudioBuffer(&temp, i);
        result->left += temp.left * coefficients[i];
        result->right += temp.right * coefficients[i];
    }
}

// Boost the volume of the audio data
void volumeBoost(audioData *audio, float volumeFactor) {
    audio->left *= volumeFactor;
    audio->right *= volumeFactor;
}

// Sum multiple audio data
void sumAudio(audioData *result, audioData *audioIn, int length) {
    result->left = 0;
    result->right = 0;

    for (int i = 0; i < length; i++) {
        result->left += audioIn[i].left;
        result->right += audioIn[i].right;
    }
}

// Add audio data to the buffer
void addToAudioBuffer(audioData *audio) {
    audioBuffer[audioBufferIndex] = *audio;
    audioBufferIndex = (audioBufferIndex + 1) % AUDIO_BUFFER_SIZE;
}

// Get audio data from the buffer, using a decrement value relative to the current time
void getFromAudioBuffer(audioData *audio, int decr) {
    int index = (audioBufferIndex - decr) % AUDIO_BUFFER_SIZE;
    if (index < 0) {
        index += AUDIO_BUFFER_SIZE;
    }
    *audio = audioBuffer[index];
}
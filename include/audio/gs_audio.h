#ifndef __GS_AUDIO_H__
#define __GS_AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common/gs_types.h"
#include "common/gs_containers.h"

// Internal audio resource data
gs_declare_resource_type( gs_audio_source );			// Actual resource data
gs_declare_resource_type( gs_audio_instance );			// Used to instance an audio source (for multiple different instances of the same data)

typedef enum gs_audio_file_type
{
	gs_ogg = 0x00,
	gs_wav	
} gs_audio_file_type;

typedef struct gs_audio_source_t
{
	s32 channels;
	s32 sample_rate;
	void* samples;
	s32 sample_count;
} gs_audio_source_t;

typedef struct gs_audio_instance_data_t
{
	gs_resource( gs_audio_source ) src;
	f32 volume;
	b32 loop;
	b32 persistent;
	b32 playing;
	f64 sample_position;
	void* user_data;						// Any custom user data required for a specific internal/external usage
} gs_audio_instance_data_t;

_force_inline
gs_audio_instance_data_t gs_audio_instance_data_new( gs_resource( gs_audio_source ) src )
{
	gs_audio_instance_data_t inst = {0};
	inst.src = src;
	inst.volume = 0.5f;
	inst.loop = false;
	inst.persistent = false;
	inst.playing = true;
	inst.user_data = NULL;
	return inst;
}

typedef struct gs_audio_i
{
	/*============================================================
	// Audio Initilization / De-Initialization
	============================================================*/
	gs_result ( * init )( struct gs_audio_i* );
	gs_result ( * shutdown )( struct gs_audio_i* );
	gs_result ( * update )( struct gs_audio_i* );
	gs_result( * commit )( struct gs_audio_i* );

	/*============================================================
	// Audio Source
	============================================================*/
	gs_resource( gs_audio_source )( * load_audio_source_from_file )( const char* file_name );

	/*============================================================
	// Audio Instance Data
	============================================================*/
	gs_resource( gs_audio_instance )( * play )( gs_audio_instance_data_t );
	void ( * pause )( gs_resource( gs_audio_instance ) );
	void ( * resume )( gs_resource( gs_audio_instance ) );
	void ( * stop )( gs_resource( gs_audio_instance ) );
	void ( * restart )( gs_resource( gs_audio_instance ) );

	void ( * set_instance_data )( gs_resource( gs_audio_instance ), gs_audio_instance_data_t );
	f32 ( * get_volume )( gs_resource( gs_audio_instance ) );
	void ( * set_volume )( gs_resource( gs_audio_instance ), f32 );

	// All internal API specific data for audio system
	void* data;
	void* user_data;		// Any custom user data

} gs_audio_i;

extern struct gs_audio_i* gs_audio_construct();

#ifdef __cplusplus
}
#endif 	// c++

#endif // __GS_AUDIO_H__
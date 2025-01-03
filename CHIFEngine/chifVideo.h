#pragma once
#include "CommonInclude.h"
#include "chifGraphicsDevice.h"
#include "chifVector.h"
#include "chifUnorderedSet.h"

#include <memory>
#include <string>

namespace chif::video
{
	struct Video
	{
		std::string title;
		std::string album;
		std::string artist;
		std::string year;
		std::string comment;
		std::string genre;
		uint32_t padded_width = 0;
		uint32_t padded_height = 0;
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t bit_rate = 0;
		chif::graphics::VideoProfile profile = chif::graphics::VideoProfile::H264;
		chif::vector<uint8_t> sps_datas;
		chif::vector<uint8_t> pps_datas;
		chif::vector<uint8_t> slice_header_datas;
		uint32_t sps_count = 0;
		uint32_t pps_count = 0;
		uint32_t slice_header_count = 0;
		chif::graphics::GPUBuffer data_stream;
		float average_frames_per_second = 0;
		float duration_seconds = 0;
		struct FrameInfo
		{
			uint64_t offset = 0;
			uint64_t size = 0;
			float timestamp_seconds = 0;
			float duration_seconds = 0;
			chif::graphics::VideoFrameType type = chif::graphics::VideoFrameType::Intra;
			uint32_t reference_priority = 0;
			int poc = 0;
			int gop = 0;
			int display_order = 0;
		};
		chif::vector<FrameInfo> frames_infos;
		chif::vector<size_t> frame_display_order;
		uint32_t num_dpb_slots = 0;
		inline bool IsValid() const { return data_stream.IsValid(); }
	};

	struct VideoInstance
	{
		const Video* video = nullptr;
		chif::graphics::VideoDecoder decoder;
		struct DPB
		{
			chif::graphics::Texture texture;
			int subresources_luminance[17] = {};
			int subresources_chrominance[17] = {};
			int poc_status[17] = {};
			int framenum_status[17] = {};
			chif::graphics::ResourceState resource_states[17] = {};
			chif::vector<uint8_t> reference_usage;
			uint8_t next_ref = 0;
			uint8_t next_slot = 0;
			uint8_t current_slot = 0;
		} dpb;
		struct OutputTexture
		{
			chif::graphics::Texture texture;
			int subresource_srgb = -1;
			int display_order = -1;
		};
		chif::vector<OutputTexture> output_textures_free;
		chif::vector<OutputTexture> output_textures_used;
		OutputTexture output;
		int target_display_order = 0;
		int current_frame = 0;
		float time_until_next_frame = 0;
		chif::vector<chif::graphics::GPUBarrier> barriers;
		enum class Flags
		{
			Empty = 0,
			Playing = 1 << 0,
			Looped = 1 << 1,
			Mipmapped = 1 << 2,
			NeedsResolve = 1 << 3,
			InitialFirstFrameDecoded = 1 << 4,
			DecoderReset = 1 << 5,
		};
		Flags flags = Flags::Empty;
		inline bool IsValid() const { return decoder.IsValid(); }
	};

	bool CreateVideo(const std::string& filename, Video* video);
	bool CreateVideo(const uint8_t* filedata, size_t filesize, Video* video);
	bool CreateVideoInstance(const Video* video, VideoInstance* instance);

	bool IsDecodingRequired(const VideoInstance* instance, float dt);
	void UpdateVideo(VideoInstance* instance, float dt, chif::graphics::CommandList cmd);
	void ResolveVideoToRGB(VideoInstance* instance, chif::graphics::CommandList cmd);
}

template<>
struct enable_bitmask_operators<chif::video::VideoInstance::Flags> {
	static const bool enable = true;
};

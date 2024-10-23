
#define GETTEXT_DOMAIN "wesnoth-lib"

#include <iostream>
#include "config_cache.h"
#include "config.h"
// #include "filesystem.hpp"
// #include "gettext.hpp"
// #include "log.hpp"
// #include "hash.hpp"
// #include "serialization/binary_or_text.hpp"
// #include "serialization/parser.hpp"
// #include "serialization/string_utils.hpp"
// #include "game_version.hpp"

// #include <boost/algorithm/string/replace.hpp>
// #include <boost/iostreams/filter/gzip.hpp>

// static lg::log_domain log_cache("cache");
// #define ERR_CACHE LOG_STREAM(err, log_cache)
// #define LOG_CACHE LOG_STREAM(info, log_cache)
// #define DBG_CACHE LOG_STREAM(debug, log_cache)

namespace game_config
{

namespace
{

void add_builtin_defines()
{

}
}
config_cache& config_cache::instance()
{
	static config_cache cache;
	return cache;
}

config_cache::config_cache()
	: force_valid_cache_(false)
	, use_cache_(true)
	, fake_invalid_cache_(false)
{
	// To set-up initial defines map correctly
	clear_defines();
}

// const preproc_map& config_cache::get_preproc_map() const
// {
// 	return defines_map_;
// }

void config_cache::clear_defines()
{
}

// void config_cache::get_config(const std::string& file_path, config& cfg, abstract_validator* validator)
// {
// 	load_configs(file_path, cfg, validator);
// }

void config_cache::write_file(std::string file_path, const config& cfg)
{
}

// void config_cache::write_file(std::string file_path, const preproc_map& defines)
// {
// 	if(defines.empty()) {
// 		if(filesystem::file_exists(file_path)) {
// 			filesystem::delete_directory(file_path);
// 		}
// 		return;
// 	}

// 	filesystem::scoped_ostream stream = filesystem::ostream_file(file_path);
// 	config_writer writer(*stream, true, game_config::cache_compression_level);

// 	// Write all defines to stream.
// 	for(const preproc_map::value_type& define : defines) {
// 		define.second.write(writer, define.first);
// 	}
// }

void config_cache::read_file(const std::string& file_path, config& cfg)
{
}

// preproc_map& config_cache::make_copy_map()
// {
// 	preproc_map& res = config_cache_transaction::instance().get_active_map(defines_map_);
// 	// HACK: copy_map doesn't have built-in defines in some cases (issue #1924)
// 	//       or they may be out of date, so brute-force them in.
// 	add_builtin_defines(res);
// 	return res;
// }

// 

void config_cache::read_defines_file(const std::string& file_path)
{
	config cfg;
	read_file(file_path, cfg);

	// DBG_CACHE << "Reading cached defines from: " << file_path;
    std::cout << "Reading cached defines from: " << file_path << std::endl;

	// use static preproc_define::read_pair(config) to make a object
	// and pass that object config_cache_transaction::insert_to_active method
	// for(const config::any_child value : cfg.all_children_range()) {
	// 	config_cache_transaction::instance().insert_to_active(
	// 		preproc_define::read_pair(value.cfg));
	// }
}

void config_cache::read_defines_queue()
{
	const std::vector<std::string>& files = config_cache_transaction::instance().get_define_files();

	for(const std::string &p : files) {
		read_defines_file(p);
	}
}


void config_cache::set_force_invalid_cache(bool force)
{
	fake_invalid_cache_ = force;
}

void config_cache::set_use_cache(bool use)
{
	use_cache_ = use;
}

void config_cache::set_force_valid_cache(bool force)
{
	force_valid_cache_ = force;
}

void config_cache::recheck_filetree_checksum()
{
	// filesystem::data_tree_checksum(true);
}

void config_cache::add_define(const std::string& define)
{
	

}

void config_cache::remove_define(const std::string& define)
{

}

bool config_cache::clean_cache()
{

	bool status = true;


	return status;
}

bool config_cache::purge_cache()
{

	bool status = true;

	return status;
}

bool config_cache::delete_cache_files(const std::vector<std::string>& paths,
									  const std::string& exclude_pattern)
{
	const bool delete_everything = exclude_pattern.empty();
	bool status = true;

	return status;
}

config_cache_transaction::state config_cache_transaction::state_ = FREE;
config_cache_transaction* config_cache_transaction::active_ = 0;

config_cache_transaction::config_cache_transaction()
	: define_filenames_()
	// , active_map_()
{
	assert(state_ == FREE);
	state_ = NEW;
	active_ = this;
}

config_cache_transaction::~config_cache_transaction()
{
	state_ = FREE;
	active_ = 0;
}

void config_cache_transaction::lock()
{
	state_ = LOCKED;
}

const std::vector<std::string>& config_cache_transaction::get_define_files() const
{
	return define_filenames_;
}

void config_cache_transaction::add_define_file(const std::string& file)
{
	define_filenames_.push_back(file);
}

// preproc_map& config_cache_transaction::get_active_map(const preproc_map& defines_map)
// {
// 	if(active_map_.empty()) {
// 		active_map_.insert(defines_map.begin(), defines_map.end());
// 		if(get_state() == NEW) {
// 			state_ = ACTIVE;
// 		}
// 	}

// 	return active_map_;
// }

namespace
{

bool compare_define()
{

	return false;
}

} // end anonymous namespace

// void config_cache_transaction::add_defines_map_diff(preproc_map& new_map)
// {
// 	if(get_state() == ACTIVE) {
// 		preproc_map temp;
// 		std::set_difference(new_map.begin(),
// 				new_map.end(),
// 				active_map_.begin(),
// 				active_map_.end(),
// 				std::insert_iterator<preproc_map>(temp,temp.begin()),
// 				&compare_define);

// 		for(const preproc_map::value_type &def : temp) {
// 			insert_to_active(def);
// 		}

// 		temp.swap(new_map);
// 	} else if (get_state() == LOCKED) {
// 		new_map.clear();
// 	}
// }

// void config_cache_transaction::insert_to_active(const preproc_map::value_type& def)
// {
// 	active_map_[def.first] = def.second;
// }

}

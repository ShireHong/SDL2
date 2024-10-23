
#include "config.h"

// #include "formatter.hpp"
// #include "lexical_cast.hpp"
// #include "log.hpp"
// #include "deprecation.hpp"
// #include "game_version.hpp"
// #include "serialization/string_utils.hpp"

#include <algorithm>
#include <cstring>

// static lg::log_domain log_config("config");
// #define ERR_CF LOG_STREAM(err, log_config)
// #define DBG_CF LOG_STREAM(debug, log_config)

// static lg::log_domain log_wml("wml");
// #define ERR_WML LOG_STREAM(err, log_wml)

namespace
{
// std::map::operator[] does not support heterogeneous lookup so we need this to work around.
template<typename Map, typename Key>
typename Map::mapped_type& map_get(Map& map, Key&& key)
{
	auto res = map.lower_bound(key);

	if(res == map.end() || key != res->first) {
		res = map.emplace_hint(res, std::piecewise_construct, std::forward_as_tuple(key), std::tuple<>());
	}

	return res->second;
}

// std::map::erase does not support heterogeneous lookup so we need this to work around.
template<typename Map, typename Key>
int map_erase_key(Map& map, Key&& key)
{
	auto i = map.find(key);
	if(i != map.end()) {
		map.erase(i);
		return 1;
	}
	return 0;
}

}

/* ** config implementation ** */

const char* config::diff_track_attribute = "__diff_track";

config::config()
	// : values_()
	// , children_()
	: ordered_children()
{
}

config::config(const config& cfg)
	// : values_(cfg.values_)
	// , children_()
	: ordered_children()
{
	append_children(cfg);
}

// config::config(config_key_type child)
// 	: values_()
// 	, children_()
// 	, ordered_children()
// {
// 	add_child(child);
// }

config::~config()
{
	clear();
}

config& config::operator=(const config& cfg)
{
	if(this == &cfg) {
		return *this;
	}

	config tmp(cfg);
	swap(tmp);
	return *this;
}

config::config(config&& cfg)
	// : values_(std::move(cfg.values_))
	// , children_(std::move(cfg.children_))
	: ordered_children(std::move(cfg.ordered_children))
{
}

config& config::operator=(config&& cfg)
{
	clear();
	swap(cfg);
	return *this;
}



void config::append_children(const config& cfg)
{
	// for(const any_child value : cfg.all_children_range()) {
	// 	add_child(value.key, value.cfg);
	// }
}

void config::append_children(config&& cfg)
{
#if 0
	//For some unknown reason this doesn't compile.
	if(children_.empty()) {
		//optimisation
		children_ = std::move(cfg.children_);
		ordered_children = std::move(cfg.ordered_children);
		cfg.clear_all_children();
		return;
	}
#endif
	// for(const any_child value : cfg.all_children_range()) {
	// 	add_child(value.key, std::move(value.cfg));
	// }
	cfg.clear_all_children();
}

void config::append_attributes(const config& cfg)
{
	// for(const attribute& v : cfg.values_) {
	// 	values_[v.first] = v.second;
	// }
}

void config::append_children(const config& cfg, const std::string& key)
{
	// for(const config& value : cfg.child_range(key)) {
	// 	add_child(key, value);
	// }
}

void config::append(const config& cfg)
{
	append_children(cfg);
	// for(const attribute& v : cfg.values_) {
	// 	values_[v.first] = v.second;
	// }
}

void config::append(config&& cfg)
{
	append_children(std::move(cfg));

	// if(values_.empty()) {
	// 	//optimisation.
	// 	values_ = std::move(cfg.values_);
	// }
	// else {
	// 	for(const attribute& v : cfg.values_) {
	// 		//TODO: move the attributes as well?
	// 		values_[v.first] = v.second;
	// 	}
	// }
	cfg.clear_attributes();
}

void config::append_children_by_move(config& cfg, const std::string& key)
{
}

void config::merge_children(const std::string& key)
{
}

void config::merge_children_by_attribute(const std::string& key, const std::string& attribute)
{
}


std::size_t config::all_children_count() const
{
	return ordered_children.size();
}


namespace
{
struct remove_ordered
{
	// remove_ordered(const config::child_map::iterator& iter)
	// 	: iter_(iter)
	// {
	// }

	// bool operator()(const config::child_pos& pos) const
	// {
	// 	return pos.pos == iter_;
	// }

private:
	// config::child_map::iterator iter_;
};
} // end anon namespace

void config::splice_children(config& src, const std::string& key)
{
	// child_map::iterator i_src = src.children_.find(key);
	// if(i_src == src.children_.end()) {
	// 	return;
	// }

	// src.ordered_children.erase(
	// 	std::remove_if(src.ordered_children.begin(), src.ordered_children.end(), remove_ordered(i_src)),
	// 	src.ordered_children.end());

	// child_list& dst = map_get(children_, key);
	// child_map::iterator i_dst = children_.find(key);

	// const auto before = dst.size();
	// dst.insert(dst.end(), std::make_move_iterator(i_src->second.begin()), std::make_move_iterator(i_src->second.end()));
	// src.children_.erase(i_src);
	// // key might be a reference to i_src->first, so it is no longer usable.

	// for(std::size_t j = before; j < dst.size(); ++j) {
	// 	ordered_children.emplace_back(i_dst, j);
	// }
}

// std::vector<config::child_pos>::iterator config::remove_child(const child_map::iterator& pos, std::size_t index)
// {
// 	/* Find the position with the correct index and decrement all the
// 	   indices in the ordering that are above this index. */
// 	std::size_t found = 0;
// 	for(child_pos& p : ordered_children) {
// 		if(p.pos != pos) {
// 			continue;
// 		}

// 		if(p.index == index) {
// 			found = &p - &ordered_children.front();
// 		} else if(p.index > index) {
// 			--p.index;
// 		}
// 	}

// 	// Remove from the child map.
// 	pos->second.erase(pos->second.begin() + index);

// 	// Erase from the ordering and return the next position.
// 	return ordered_children.erase(ordered_children.begin() + found);
// }

config::all_children_iterator config::erase(const config::all_children_iterator& i)
{
	// return all_children_iterator(remove_child(i.i_->pos, i.i_->index));
}

void config::clear()
{
	// No validity check for this function.
	// children_.clear();
	// values_.clear();
	ordered_children.clear();
}

void config::clear_all_children()
{
	// No validity check for this function.
	// children_.clear();
	ordered_children.clear();
}

void config::clear_attributes()
{
	// No validity check for this function.
	// values_.clear();
}

bool config::empty() const
{
	// return children_.empty() && values_.empty();
}

config::all_children_iterator::reference config::all_children_iterator::operator*() const
{
	// return any_child(&i_->pos->first, i_->pos->second[i_->index].get());
}

config::const_all_children_iterator::reference config::const_all_children_iterator::operator*() const
{
	// return any_child(&i_->pos->first, i_->pos->second[i_->index].get());
}

config::const_all_children_iterator config::ordered_begin() const
{
	return const_all_children_iterator(ordered_children.cbegin());
}

config::const_all_children_iterator config::ordered_cbegin() const
{
	return const_all_children_iterator(ordered_children.cbegin());
}

config::const_all_children_iterator config::ordered_end() const
{
	return const_all_children_iterator(ordered_children.cend());
}

config::const_all_children_iterator config::ordered_cend() const
{
	return const_all_children_iterator(ordered_children.cend());
}

config::all_children_iterator config::ordered_begin()
{
	return all_children_iterator(ordered_children.begin());
}

config::all_children_iterator config::ordered_end()
{
	return all_children_iterator(ordered_children.end());
}


config config::get_diff(const config& c) const
{
	config res;
	get_diff(c, res);
	return res;
}

void config::get_diff(const config& c, config& res) const
{
	config* inserts = nullptr;

	// for(const auto& v : values_) {
	// 	if(v.second.blank()) {
	// 		continue;
	// 	}

	// 	const attribute_map::const_iterator j = c.values_.find(v.first);
	// 	if(j == c.values_.end() || (v.second != j->second && !v.second.blank())) {
	// 		if(inserts == nullptr) {
	// 			inserts = &res.add_child("insert");
	// 		}

	// 		(*inserts)[v.first] = v.second;
	// 	}
	// }

	// config* deletes = nullptr;

	// for(const auto& v : c.values_) {
	// 	if(v.second.blank()) {
	// 		continue;
	// 	}

	// 	const attribute_map::const_iterator itor = values_.find(v.first);
	// 	if(itor == values_.end() || itor->second.blank()) {
	// 		if(deletes == nullptr) {
	// 			deletes = &res.add_child("delete");
	// 		}

	// 		(*deletes)[v.first] = "x";
	// 	}
	// }

	std::vector<std::string> entities;

	// for(const auto& child : children_) {
	// 	entities.push_back(child.first);
	// }

	// for(const auto& child : c.children_) {
	// 	if(children_.count(child.first) == 0) {
	// 		entities.push_back(child.first);
	// 	}
	// }

	for(const std::string& entity : entities) {
		// const child_map::const_iterator itor_a = children_.find(entity);
		// const child_map::const_iterator itor_b = c.children_.find(entity);

		static const child_list dummy;

		// Get the two child lists. 'b' has to be modified to look like 'a'.
		// const child_list& a = itor_a != children_.end() ? itor_a->second : dummy;
		// const child_list& b = itor_b != c.children_.end() ? itor_b->second : dummy;

		std::size_t ndeletes = 0;
		std::size_t ai = 0, bi = 0;
		// while(ai != a.size() || bi != b.size()) {
		// 	// If the two elements are the same, nothing needs to be done.
		// 	if(ai < a.size() && bi < b.size() && *a[ai] == *b[bi]) {
		// 		++ai;
		// 		++bi;
		// 	} else {
		// 		// We have to work out what the most appropriate operation --
		// 		// delete, insert, or change is the best to get b[bi] looking like a[ai].
		// 		// std::stringstream buf;

		// 		// If b has more elements than a, then we assume this element
		// 		// is an element that needs deleting.
		// 		// if(b.size() - bi > a.size() - ai) {
		// 		// 	config& new_delete = res.add_child("delete_child");
		// 		// 	buf << bi - ndeletes;
		// 		// 	new_delete.values_["index"] = buf.str();
		// 		// 	new_delete.add_child(entity);

		// 		// 	++ndeletes;
		// 		// 	++bi;
		// 		// }

		// 		// If b has less elements than a, then we assume this element
		// 		// is an element that needs inserting.
		// 		// else if(b.size() - bi < a.size() - ai) {
		// 		// 	config& new_insert = res.add_child("insert_child");
		// 		// 	buf << ai;
		// 		// 	new_insert.values_["index"] = buf.str();
		// 		// 	new_insert.add_child(entity, *a[ai]);

		// 		// 	++ai;
		// 		// }

		// 		// Otherwise, they have the same number of elements,
		// 		// so try just changing this element to match.
		// 		// else {
		// 		// 	config& new_change = res.add_child("change_child");
		// 		// 	buf << bi;
		// 		// 	new_change.values_["index"] = buf.str();
		// 		// 	new_change.add_child(entity, a[ai]->get_diff(*b[bi]));

		// 		// 	++ai;
		// 		// 	++bi;
		// 		// }
		// 	}
		// }
	}
}

void config::apply_diff(const config& diff, bool track /* = false */)
{
	// if(track) {
	// 	values_[diff_track_attribute] = "modified";
	// }

	
}

void config::clear_diff_track(const config& diff)
{
	
}


void config::merge_attributes(const config& cfg)
{
	// assert(this != &cfg);
	// for(const attribute& v : cfg.values_) {
	// 	std::string key = v.first;
	// 	if(key.substr(0, 7) == "add_to_") {
	// 		std::string add_to = key.substr(7);
	// 		values_[add_to] = values_[add_to].to_double() + v.second.to_double();
	// 	} else if(key.substr(0, 10) == "concat_to_") {
	// 		std::string concat_to = key.substr(10);
	// 		// TODO: Only use t_string if one or both are actually translatable?
	// 		// That probably requires using a visitor though.
	// 		values_[concat_to] = values_[concat_to].t_str() + v.second.t_str();
	// 	} else {
	// 		values_[v.first] = v.second;
	// 	}
	// }
}
/**
 * Merge config 'c' into this config, overwriting this config's values.
 */
void config::merge_with(const config& c)
{
	std::vector<child_pos> to_remove;
	std::map<std::string, unsigned> visitations;

	// Merge attributes first
	merge_attributes(c);

	// Now merge shared tags
	all_children_iterator::Itor i, i_end = ordered_children.end();
	for(i = ordered_children.begin(); i != i_end; ++i) {
		// const std::string& tag = i->pos->first;
		// const child_map::const_iterator j = c.children_.find(tag);

		// if(j != c.children_.end()) {
		// 	unsigned& visits = visitations[tag];

		// 	if(visits < j->second.size()) {
		// 		// Get a const config so we do not add attributes.
		// 		const config& merge_child = *j->second[visits++];

		// 		// if(merge_child["__remove"].to_bool()) {
		// 		// 	to_remove.push_back(*i);
		// 		// } else {
		// 		// 	(i->pos->second[i->index])->merge_with(merge_child);
		// 		// }
		// 	}
		// }
	}

	// Now add any unvisited tags
	// for(const auto& pair : c.children_) {
	// 	const std::string& tag = pair.first;
	// 	unsigned& visits = visitations[tag];
	// 	while(visits < pair.second.size()) {
	// 		add_child(tag, *pair.second[visits++]);
	// 	}
	// }

	// Remove those marked so
// 	std::map<std::string, std::size_t> removals;
// 	for(const child_pos& pos : to_remove) {
// 		const std::string& tag = pos.pos->first;
// 		auto& removes = removals[tag];
// 		remove_child(tag, pos.index - removes++);
// 	}
}

/**
 * Merge config 'c' into this config, preserving this config's values.
 */
void config::inherit_from(const config& c)
{
	// Using a scratch config and merge_with() seems to execute about as fast
	// as direct coding of this merge.
	config scratch(c);
	scratch.merge_with(*this);
	swap(scratch);
}

/**
 * Merge the attributes of config 'c' into this config, preserving this config's values.
 */
void config::inherit_attributes(const config& cfg)
{
	// for(const attribute& v : cfg.values_) {
	// 	attribute_value& v2 = values_[v.first];
	// 	if(v2.blank()) {
	// 		v2 = v.second;
	// 	}
	// }
}
bool config::matches(const config& filter) const
{
	bool result = true;

	// for(const attribute& i : filter.attribute_range()) {
	// 	if(i.first.compare(0, 8, "glob_on_") == 0) {
	// 		const attribute_value* v = get(i.first.substr(8));
	// 		if(!v || !utils::wildcard_string_match(v->str(), i.second.str())) {
	// 			result = false;
	// 			break;
	// 		}
	// 	} else {
	// 		const attribute_value* v = get(i.first);
	// 		if(!v || *v != i.second) {
	// 			result = false;
	// 			break;
	// 		}
	// 	}
	// }

	// for(const any_child i : filter.all_children_range()) {
	// 	if(i.key == "not") {
	// 		result = result && !matches(i.cfg);
	// 		continue;
	// 	} else if(i.key == "and") {
	// 		result = result && matches(i.cfg);
	// 		continue;
	// 	} else if(i.key == "or") {
	// 		result = result || matches(i.cfg);
	// 		continue;
	// 	}

	// 	bool found = false;
	// 	for(const config& j : child_range(i.key)) {
	// 		if(j.matches(i.cfg)) {
	// 			found = true;
	// 			break;
	// 		}
	// 	}

	// 	result = result && found;
	// }

	return result;
}

// std::string config::debug() const
// {
// 	std::ostringstream outstream;
// 	outstream << *this;
// 	return outstream.str();
// }

// std::ostream& operator<<(std::ostream& outstream, const config& cfg)
// {
// 	static int i = 0;
// 	i++;

// 	for(const config::attribute& val : cfg.attribute_range()) {
// 		if(val.second.blank()) {
// 			continue;
// 		}

// 		for(int j = 0; j < i - 1; j++) {
// 			outstream << '\t';
// 		}

// 		outstream << val.first << " = " << val.second << '\n';
// 	}

// 	for(const config::any_child child : cfg.all_children_range()) {
// 		for(int j = 0; j < i - 1; ++j) {
// 			outstream << '\t';
// 		}

// 		outstream << "[" << child.key << "]\n";
// 		outstream << child.cfg;

// 		for(int j = 0; j < i - 1; ++j) {
// 			outstream << '\t';
// 		}

// 		outstream << "[/" << child.key << "]\n";
// 	}

// 	i--;
// 	return outstream;
// }

std::string config::hash() const
{
	static const unsigned int hash_length = 128;
	static const char hash_string[] = "+-,.<>0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char hash_str[hash_length + 1];

	unsigned int i;
	for(i = 0; i != hash_length; ++i) {
		hash_str[i] = 'a';
	}

	hash_str[hash_length] = 0;

	i = 0;
	// for(const attribute& val : values_) {
	// 	if(val.second.blank()) {
	// 		continue;
	// 	}

	// 	for(char c : val.first) {
	// 		hash_str[i] ^= c;
	// 		if(++i == hash_length) {
	// 			i = 0;
	// 		}
	// 	}

	// 	std::string base_str = val.second.t_str().base_str();
	// 	for(const char c : base_str) {
	// 		hash_str[i] ^= c;
	// 		if(++i == hash_length) {
	// 			i = 0;
	// 		}
	// 	}
	// }

	// for(const any_child ch : all_children_range()) {
	// 	std::string child_hash = ch.cfg.hash();
	// 	for(char c : child_hash) {
	// 		hash_str[i] ^= c;
	// 		++i;
	// 		if(i == hash_length) {
	// 			i = 0;
	// 		}
	// 	}
	// }

	for(i = 0; i != hash_length; ++i) {
		hash_str[i] = hash_string[static_cast<unsigned>(hash_str[i]) % strlen(hash_string)];
	}

	return std::string(hash_str);
}

void config::swap(config& cfg)
{
	// values_.swap(cfg.values_);
	// children_.swap(cfg.children_);
	ordered_children.swap(cfg.ordered_children);
}

void swap(config& lhs, config& rhs)
{
	lhs.swap(rhs);
}

// bool config::validate_wml() const
// {
// 	return std::all_of(children_.begin(), children_.end(), [](const auto& pair)
// 	{
// 		return valid_tag(pair.first) &&
// 			std::all_of(pair.second.begin(), pair.second.end(),
// 			[](const auto& c) { return c->validate_wml(); });
// 	}) &&
// 		std::all_of(values_.begin(), values_.end(), [](const auto& pair)
// 	{
// 		return valid_attribute(pair.first);
// 	});
// }

bool operator==(const config& a, const config& b)
{
	// if(a.values_ != b.values_) {
	// 	return false;
	// }

	// config::const_all_children_itors x = a.all_children_range(), y = b.all_children_range();
	// for(; !x.empty() && !y.empty(); x.pop_front(), y.pop_front()) {
	// 	if(x.front().key != y.front().key || x.front().cfg != y.front().cfg) {
	// 		return false;
	// 	}
	// }

	//return x.empty() && y.empty();
    return true;
}

#pragma once

#include "common/MuiError.h"

#include "thirdparty/jsoncpp/json.h"

#include <memory>

template <typename T> class JsonableBase {
public:
    JsonableBase(const T& obj) : obj_(new T(obj)), json_(nullptr) {}
	JsonableBase(const JsonCpp::Value& json) : obj_(nullptr), json_(new JsonCpp::Value(json)) {}

	JsonCpp::Value toJson() const { 
		if (json_ == nullptr) {
			json_.reset(new JsonCpp::Value(fromObject(*obj_)));
		}
        return *json_;
	}
	T toObject() const {
		if (obj_ == nullptr) {
			checkValidity();
			obj_.reset(new T(fromJson(*json_)));
		}
		return *obj_;
	}
	
	operator JsonCpp::Value() const { return toJson(); }
    operator T() const { return toObject(); }

protected:
    JsonableBase() {}
    virtual T fromJson(const JsonCpp::Value& json) const = 0;
	virtual JsonCpp::Value fromObject(const T& obj) const = 0;
	virtual std::vector<std::string> requiredMembers() const = 0;

private:
	void checkValidity() const {
		for (auto member : requiredMembers()) {
			if (!json_->isMember(member)) {
				throw MuiError("Required member '" + member + "' is missing in JSON definition", __FILE__, __LINE__);
			}
		}
	}

private:
    mutable std::unique_ptr<T> obj_;
	mutable std::unique_ptr<JsonCpp::Value> json_;
};

template<typename T> class Jsonable;
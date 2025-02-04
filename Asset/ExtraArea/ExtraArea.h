#pragma once
#ifndef VISUALNOVEL_EXTRAAREA_H
#define VISUALNOVEL_EXTRAAREA_H
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace vn
{
	namespace asset
	{
    /*
        Sync with AssetPack and SaveFile
        In order to deal with the ProgramIndex not being used for store data
    */
        class ExtraArea
        {
        private:
            std::map<std::string, int64_t> string_define_index_to_value_;

            inline static constexpr double version_ = 1.0;
        public:
            ExtraArea(const std::vector<char>& data)
            {

            }
            ExtraArea() = default;

            std::vector<char> tranformToData() const
            {
                return {};
            }
            static std::shared_ptr<const ExtraArea> loadFromData(const std::vector<char>& data)
            {
                return std::make_shared<const ExtraArea>(data);
            }
            static std::shared_ptr<ExtraArea> createNew()
            {
                return std::make_shared<ExtraArea>();
            }
        };
	}
}

#endif // VISUALNOVEL_EXTRAAREA_H

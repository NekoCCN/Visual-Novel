#include "ThemePackCreator.h"

bool vn::asset::theme::ThemePackCreatorInterface::operator()()
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Start to make theme file");
	uint64_t buf = 0;
	uint64_t sum = 0;

	// Label
	SDL_WriteIO(wstream_, label_.data(), label_.size() + 1);

	toc_size_ = ThemeAssetList::theme_asset_list_size_ + 1;
	SDL_WriteIO(wstream_, &toc_size_, sizeof(uint64_t));

	toc_.resize(toc_size_, 0);
	SDL_WriteIO(wstream_, toc_.data(), toc_size_ * sizeof(uint64_t));
	toc_.resize(1, 0);


	for (int i = 0; i < toc_size_ - 1; ++i)
	{
		std::string file_name = ThemeAssetList::to_string(static_cast<ThemeAssetList::ThemeAssetListIndex>(i));

		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Add asset : %s", file_name.c_str());

		if (!SDL_GetStorageFileSize(storage_, file_name.c_str(), &buf))
		{
			throw core::exception::file_not_found_error();
		}
		sum += buf;
		toc_.push_back(sum);

		char* buffer = new char[buf];
		
		SDL_ReadStorageFile(storage_, file_name.c_str(), buffer, buf);
		SDL_WriteIO(wstream_, buffer, buf);

		delete[] buffer;
	}

	SDL_SeekIO(wstream_, sizeof(uint64_t) + label_.size() + 1, SDL_IO_SEEK_SET);
	SDL_WriteIO(wstream_, toc_.data(), (toc_size_ + 1) * sizeof(uint64_t));

	return true;
}
vn::asset::theme::ThemePackCreator::ThemePackCreator(const std::string& file_path, const std::string& storage_path)
{
	// File IO
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to create theme file : %s", file_path.c_str());
	wstream_ = SDL_IOFromFile(file_path.c_str(), "rb");
	if (wstream_ != nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "File name exist : %s", file_path.c_str());
		SDL_CloseIO(wstream_);
		throw core::exception::file_existed_error();
	}
	SDL_CloseIO(wstream_);

	wstream_ = SDL_IOFromFile(file_path.c_str(), "wb");
	if (wstream_ == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can not create theme file");
		throw core::exception::create_file_error();
	}

	// Storage
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Try to open theme dir : %s", storage_path.c_str());
	storage_ = SDL_OpenFileStorage(storage_path.c_str());
	if (storage_ == nullptr || !SDL_StorageReady(storage_))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can not open theme dir");
		throw core::exception::storage_not_found_error();
	}
}
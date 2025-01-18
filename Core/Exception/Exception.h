#pragma once
#pragma execution_character_set("utf-8")
#ifndef VISUALNOVEL_CORE_EXCEPTION_H
#define VISUALNOVEL_CORE_EXCEPTION_H
#include <exception>
#include <SDL3/SDL_messagebox.h>

namespace vn
{
	namespace core
	{
		namespace exception
		{
			void exception_hinding();

			class logsis_init_error : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][LogSis] LogSis Init Error!"; }
			};
			class unknown_format_error : public std::exception
			{
			public:
				virtual const char* what() { return "[ERROR][Asset] Unknown format."; }
			};
			class syntax_error_error : public std::exception
			{
			public:
				virtual const char* what() { return "[ERROR][Asset] Syntax Error."; }
			};
			class file_not_found_error : public std::exception
			{
			public:
				virtual const char* what() { return "[ERROR][Asset] File do not exist."; }
			};
			class storage_not_found_error : public std::exception
			{
			public:
				virtual const char* what() { return "[ERROR][Asset] Storage do not exist."; }
			};
			class file_existed_error : public std::exception
			{
			public:
				virtual const char* what() { return "[ERROR][Asset] File is existed."; }
			};
			class asset_manage_init_error : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Asset] Asset Manage Init Error!"; }
			};
			class invalid_file_format : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Asset] Invalid file format!"; }
			};
			class renderer_do_not_support : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Video] VisualText does not support this system!"; }
			};
			class path_not_exists : public asset_manage_init_error
			{
			public:
				const char* what() override { return "[FATAL][Asset] Asset path is not found!"; }
			};
			class SDL_error : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Video] Windows System Error!"; }
				static const char* getErrorMsg() { return SDL_GetError(); }
			};
			class SDL_init_error : public SDL_error
			{
			public:
				const char* what() override { return "[FATAL][Video] Windows System Init Error!"; }
			};
			class monitor_index_error : public SDL_error
			{
			public:
				const char* what() override { return "[FATAL][Video] Invalid monitor index"; }
			};
			class create_window_error : public SDL_error
			{
			public:
				const char* what() override { return "[FATAL][Video] Can`t create window!"; }
			};
			class create_file_error : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Application] Can`t create file!"; }
			};
			class out_of_memory : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Asset] Out of memory!"; }
			};
			class can_not_read_file : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Asset] Can`t read file!"; }
			};
			class asset_index_error : public std::exception
			{
			public:
				virtual const char* what() { return "[FATAL][Asset] File index error!"; }
			};
		}
	}
}
#endif
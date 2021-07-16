#include <iostream>

#include "../../inc/base/logger.h"

using joszva::engine::logger;

logger* logger::instance()
{
    static logger log;
    return &log;
}

void logger::trace(const std::string& txt)
{
    log(log_level::TRACE, txt);
}

void logger::warning(const std::string& txt)
{
    log(log_level::WARNING, txt);
}

static bool is_file_empty(std::ifstream& file)
{
    return file.peek() == std::ifstream::traits_type::eof();
}

void logger::log(log_level level, const std::string& txt)
{
    const char* level_txt = nullptr;
    switch (level)
    {
        case log_level::TRACE: level_txt = "[TRACE]"; break;
        case log_level::WARNING: level_txt = "[WARNING]"; break;
        case log_level::ERROR: level_txt = "[ERROR]"; break;
        case log_level::FATAL: level_txt = "[FATAL]"; break;
    }

    std::cout << level_txt << " " << txt << std::endl;

    input_file.open("engine.txt");
    /* check if the log file already exists */
    if (input_file.is_open())
    {
        static int first_open_check = 0;
        while (input_file.is_open())
        {
            /* simple check to see if the file is being opened for the first time */
            if (first_open_check == 0)
            {
                output_file.open("engine.txt", std::ios::trunc);
                output_file << level_txt << " " << txt << std::endl;
                output_file.close();
                ++first_open_check;
            }
            else if (!is_file_empty(input_file))
            {
                output_file.open("engine.txt", std::ios::app);
                output_file << level_txt << " " << txt << std::endl;
                output_file.close();
            }

            input_file.close();
        }
    }
    /* if the log file doesn't exist */
    else 
    {
        output_file.open("engine.txt", std::ios::app);
        while (output_file.is_open())
        {
            output_file << level_txt << " " << txt << std::endl;
            output_file.close();
        }
    }
}
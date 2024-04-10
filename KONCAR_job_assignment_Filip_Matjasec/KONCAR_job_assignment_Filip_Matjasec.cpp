#include <cstdio>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <vector>
#include <initializer_list>
#include <iomanip>
#include <map>
#include <queue>
#include <sstream>
#include <stack>

namespace koncar {
    // Task 1 - Version 1
    //****************************************************************
    /**
     * @brief Adds elements to a container using variadic arguments.
     *
     * This function adds elements to the specified container using variadic arguments.
     * It accepts any number of arguments and appends them to the end of the container.
     * The type of container must support the `emplace_back` operation.
     * We utilize a fold expression (left-hand side) introduced in C++17 to efficiently add elements to the container.
     *
     * @tparam Container The type of container to which elements will be added.
     * @tparam Args The types of elements to be added to the container.
     * @param container The container to which elements will be added.
     * @param args The elements to be added to the container.
     *
     * @details This function utilizes a variadic function template to accept any number of arguments
     * and adds them to the end of the container using the `emplace_back` operation.
     * It is suitable for situations where a variable number of elements need to be appended to a container which supports the 'emplace_back' function.
     *
     * Example usage:
     * \code{.cpp}
     * std::vector<int> vector;
     * add_range(vector, 1, 2, 3, 4, 5);
     * // vector now contains {1, 2, 3, 4, 5}
     * \endcode
     */
    template <typename Container, typename... Args>
    void add_range(Container& container, Args&&... args)
    {
        (container.emplace_back(std::forward<Args>(args)), ...);
    }

    // Task 1 - Version 2
    //****************************************************************
    /**
     * @brief Adds elements to a container using an initializer list.
     *
     * This function adds elements to the specified container using an initializer list.
     * It inserts elements from the initializer list at the end of the container.
     *
     * @tparam Container The type of container to which elements will be added.
     * @tparam T The type of elements in the initializer list.
     * @param container The container to which elements will be added.
     * @param values The initializer list containing elements to be added to the container.
     *
     * @details This function takes an initializer list of elements and inserts them at the end of the container.
     * It is suitable for situations where a set of elements needs to be appended to a container using an initializer list.
     *
     * Example usage:
     * \code{.cpp}
     * std::vector<int> vector;
     * add_to_container(vector, {1, 2, 3, 4, 5});
     * // vector now contains {1, 2, 3, 4, 5}
     * \endcode
     */
    template <typename Container, typename T>
    void add_to_container(Container& container, std::initializer_list<T> values)
    {
        container.insert(container.end(), values.begin(), values.end());
    }
    
    // Task 2.1
    //****************************************************************
    /**
     * @brief Converts a binary data vector to a hexadecimal string representation.
     *
     * This function takes a vector of binary data and converts it into a hexadecimal string representation.
     * Each byte in the input vector is converted to its hexadecimal representation and appended to the output string.
     *
     * @param data The vector of binary data to be converted to a string.
     * @param uppercase Flag indicating whether the resulting hexadecimal string should be in uppercase (default is true).
     * @return A hexadecimal string representation of the binary data.
     *
     * @details This function iterates through each byte in the input data vector, converts it to hexadecimal,
     * and appends it to the output string. The resulting string contains the hexadecimal representation of the binary data.
     * The optional parameter `uppercase` determines whether the hexadecimal characters should be in uppercase.
     *
     * Example usage:
     * \code{.cpp}
     * const std::vector<uint8_t> binary_data = { 0xBA, 0xAD, 0xF0, 0x0D };
     * const std::string hex_string = koncar::binary_to_string(binary_data);
     * // hex_string contains "BAADF00D"
     * \endcode
     */
    std::string binary_to_string(const std::vector<uint8_t>& data, const bool uppercase = true) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        if (uppercase)
            oss << std::uppercase;
        for (const auto byte : data)
            oss << std::setw(2) << static_cast<int>(byte);
        return oss.str();
    }
    
    // Task 2.2
    //****************************************************************
    /**
     * @brief Converts a hexadecimal string to a binary data vector.
     *
     * This function takes a hexadecimal string and converts it into a vector of binary data.
     * Each pair of hexadecimal characters in the input string represents one byte of binary data,
     * which is then appended to the output vector.
     *
     * @param str The hexadecimal string to be converted to binary data.
     * @return A vector of binary data representing the input hexadecimal string.
     *
     * @details This function iterates through the input string, processing each pair of hexadecimal characters
     * and converting them into their corresponding binary representation. The resulting vector contains
     * the binary representation of the input hexadecimal string.
     *
     * Example usage:
     * \code{.cpp}
     * const std::string hex_string = "BAADF00D";
     * const std::vector<uint8_t> binary_data = string_to_binary(hex_string);
     * // binary_data contains { 0xBA, 0xAD, 0xF0, 0x0D }
     * \endcode
     */
    std::vector<uint8_t> string_to_binary(const std::string& str) {
        std::vector<uint8_t> result;
        for (size_t i = 0; i < str.size(); i += 2) {
            std::istringstream iss(str.substr(i, 2));
            int byte;
            iss >> std::hex >> byte;
            result.push_back(static_cast<uint8_t>(byte));
        }
        return result;
    }

    // Task 3
    //****************************************************************
    /**
     * @brief Computes the total size of all files and directories within a specified directory and its subdirectories.
     *
     * This function iterates over all files and directories within the specified directory and its subdirectories,
     * accumulating the sizes of all encountered files. It handles errors that may occur during traversal or while
     * obtaining file sizes.
     *
     * @param path The path to the directory for which the size is to be computed.
     * @return The total size, in bytes, of all files and directories within the specified directory and its subdirectories.
     *
     * @details This function traverses the directory structure specified by the input path, including all subdirectories,
     * and computes the cumulative size of all files encountered. It considers both files and directories in the calculation.
     * Any errors encountered during traversal or while obtaining file sizes are caught and output to std::cerr.
     *
     * Example usage:
     * \code{.cpp}
     * const std::filesystem::path directory_path = "/path/to/directory";
     * const uint64_t total_size = directory_size(directory_path);
     * // total_size contains the combined size of all files and directories within the specified directory and its subdirectories
     * \endcode
     */
    uint64_t directory_size(const std::filesystem::path& path) {
        uint64_t size = 0;
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                try {
                    size += std::filesystem::file_size(entry);
                } catch (const std::filesystem::filesystem_error& ex) {
                    // Handle error while getting file size
                    std::cerr << "Error getting file size: " << ex.what() << std::endl;
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            // Handle error while iterating over directory
            std::cerr << "Error iterating directory: " << ex.what() << std::endl;
        }
        return size;
    }
}


int main(int argc, char* argv[])
{
    const std::filesystem::path directory_path = "C:\\Users\\fmatj\\OneDrive\\Desktop\\Blueprints";
    try {
        const uint64_t size = koncar::directory_size(directory_path);
        std::cout << "Directory size: " << size << " bytes" << std::endl;
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

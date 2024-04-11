#include <deque>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <vector>
#include <initializer_list>
#include <iomanip>
#include <sstream>
#include <vector>

namespace koncar {

    // namespace alias for std::filesystem
    namespace fs = std::filesystem;
    
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
     * By using a container rvalue reference (Container&&), this function can handle:
     * 1. In-place construction
     * 2. Modifying existing containers
     * 
     * Example usage:
     * \code{.cpp}
     * std::vector<int> vector;
     * koncar::add_range(vector, 1, 2, 3, 4, 5);
     * // vector now contains {1, 2, 3, 4, 5}
     * \endcode
     */
    template <typename Container, typename... Args>
    void add_range(Container&& container, Args&&... args) {
        // If the container is an array
        // if constexpr (std::is_array_v<std::remove_reference_t<Container>>) {
        //     // Use indexing to assign values directly
        //     constexpr std::size_t container_size = std::extent_v<std::remove_reference_t<Container>>;
        //     static_assert(sizeof...(args) <= container_size, "Too many elements to add to the array");
        //     std::size_t i = 0;
        //     ((container[i++] = std::forward<Args>(args)), ...);
        // }
        // else {
            (container.emplace_back(std::forward<Args>(args)), ...);
       // }
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
     * By using a container rvalue reference (Container&&), this function can handle:
     * 1. In-place construction
     * 2. Modifying existing containers
     *
     * Example usage:
     * \code{.cpp}
     * std::vector<int> vector;
     * koncar::add_range(vector, {1, 2, 3, 4, 5});
     * // vector now contains {1, 2, 3, 4, 5}
     * \endcode
     */
    template <typename Container, typename T>
    void add_range(Container&& container, std::initializer_list<T> values) {
        container.insert(container.end(), values.begin(), values.end());
    }
    
    // Task 2.1
    //****************************************************************
    /**
     * @brief Converts binary data to a hexadecimal string representation.
     *
     * This function takes a vector of binary data and converts it into a hexadecimal string representation.
     * Each byte in the input vector is converted to its hexadecimal representation and concatenated to form the output string.
     * If an error occurs during the conversion process, an exception of type std::exception is thrown.
     * The function handles the exception and returns an empty string to indicate failure.
     *
     * @param data The vector of binary data to be converted to a hexadecimal string.
     * @param uppercase Optional flag indicating whether the resulting hexadecimal string should be in uppercase (default is true).
     * @return A hexadecimal string representation of the input binary data, or an empty string if conversion fails.
     *
     * @details This function iterates through each byte in the input vector, converts it to hexadecimal,
     * and appends it to the output string. The resulting string contains the hexadecimal representation of the binary data.
     * Optionally, the function allows specifying whether the hexadecimal characters should be in uppercase.
     * Any errors encountered during the conversion process are caught and handled.
     *
     * Example usage:
     * \code{.cpp}
     * const std::vector<uint8_t> binary_data = { 0xBA, 0xAD, 0xF0, 0x0D };
     * const std::string hex_string = koncar::binary_to_string(binary_data);
     * // hex_string contains "BAADF00D"
     * \endcode
     */
    std::string binary_to_string(const std::vector<uint8_t>& data, const bool uppercase = true) {
        try {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');
            if (uppercase)
                oss << std::uppercase;
            for (const auto byte : data)
                oss << std::setw(2) << static_cast<int>(byte);
            return oss.str();
        } catch (const std::exception& e) {
            // Handle the exception
            std::cerr << "Error: " << e.what() << std::endl;
            // Return an empty string to indicate failure
            return "";
        }
    }
    
    // Task 2.2
    //****************************************************************
    /**
     * @brief Converts a hexadecimal string to binary data.
     *
     * This function takes a hexadecimal string and converts it into a vector of binary data.
     * Each pair of hexadecimal characters in the input string represents one byte of binary data,
     * which is then appended to the output vector.
     * If the input string contains invalid hexadecimal characters or results in bytes outside the range [0, 255],
     * or if the input string does not contain an even number of characters, exception of type std::invalid_argument if thrown. 
     * Any exceptions thrown during the conversion process are caught locally, and an empty vector is returned
     * to indicate failure.
     *
     * @param str The hexadecimal string to be converted to binary data.
     * @return A vector of binary data representing the input hexadecimal string, or an empty vector if conversion fails.
     *
     * @details This function iterates through the input string, extracting pairs of hexadecimal characters
     * and converting them into their corresponding binary representation. Each byte is then pushed into the output vector.
     * It checks for the even length of the input string and ensures that each character in the substring
     * contains valid hexadecimal characters. Any invalid characters or odd-length input strings result in an exception,
     * which is caught and handled.
     *
     * Example usage:
     * \code{.cpp}
     * const std::string hex_string = "BAADF00D";
     * std::vector<uint8_t> binary_data = koncar::string_to_binary(hex_string);
     * // binary_data contains { 0xBA, 0xAD, 0xF0, 0x0D }
     * \endcode
     */
    std::vector<uint8_t> string_to_binary(const std::string& str) {
        try {
            // Check if the input string has an even length
            if (str.size() & 1) {
                throw std::invalid_argument("Input string length must be even");
            }
        
            std::vector<uint8_t> result;
            for (size_t i = 0; i < str.size(); i += 2) {
                std::string substr = str.substr(i, 2);
                // Check if the substring contains valid hexadecimal characters
                for (const char c : substr) {
                    if (!std::isxdigit(c)) {
                        throw std::invalid_argument("Invalid hexadecimal character: " + std::string(1, c));
                    }
                }
                std::istringstream iss(substr);
                int byte;
                iss >> std::hex >> byte;
                result.push_back(static_cast<uint8_t>(byte));
            }
            return result;
        } catch (const std::exception& e) {
            // Handle the exception
            std::cerr << "Error: " << e.what() << std::endl;
            // Return an empty vector to indicate failure
            return {};
        }
    }

    // Task 3 - Version 1
    //****************************************************************
    /**
     * @brief Computes the total size of all files and directories within a specified directory and its subdirectories
     * using recursive_directory_iterator.
     *
     * This function iterates over all files and directories within the specified directory and its subdirectories,
     * accumulating the sizes of all encountered files. Errors that occur during traversal or while processing individual
     * entries are handled.
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
     * const koncar::fs::path directory_path = "Path\\ToFile";
     * const uint64_t total_size = koncar::directory_size(directory_path);
     * // total_size contains the combined size of all files and directories within the specified directory and its subdirectories
     * \endcode
     */
    uint64_t directory_size(const fs::path& path) {
        uint64_t size = 0;
        try {
            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                try {
                    size += fs::file_size(entry);
                } catch (const fs::filesystem_error& ex) {
                    // Handle error while getting file size
                    std::cerr << "Error getting file size: " << ex.what() << std::endl;
                }
            }
        } catch (const fs::filesystem_error& ex) {
            // Handle error while iterating over directory
            std::cerr << "Error iterating directory: " << ex.what() << std::endl;
        }
        return size;
    }
    
    // Task 3 - Version 2
    //****************************************************************
    /**
     * @brief Calculates the total size of all files and directories within a directory and its subdirectories using recursion.
     *
     * This function recursively traverses the directory structure starting from the specified path,
     * accumulating the sizes of all files and directories encountered along the way.
     * Errors that occur during traversal or while processing individual entries are handled.
     *
     * @param path The path to the directory for which the size is to be calculated.
     * @return The total size, in bytes, of all files and directories within the specified directory and its subdirectories.
     *
     * @details This function recursively traverses the directory structure starting from the input path,
     * considering both files and directories in the computation of the total size.
     * When encountering a directory, it recursively calls itself to include the size of subdirectories.
     * Any errors encountered during traversal or while processing entries are caught and output to std::cerr.
     *
     * Example usage:
     * \code{.cpp}
     * const koncar::fs::path directory_path = "Path\\ToFile";
     * const uint64_t total_size = koncar::directory_size_recursive(directory_path);
     * // total_size contains the combined size of all files and directories within the specified directory and its subdirectories
     * \endcode
     */
    uint64_t directory_size_recursive(const fs::path& path) {
        uint64_t size = 0;
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                try {
                    if (fs::is_regular_file(entry)) {
                        size += fs::file_size(entry);
                    } else if (fs::is_directory(entry)) {
                        // Call recursion
                        size += directory_size_recursive(entry);
                    }
                } catch (const fs::filesystem_error& ex) {
                    // Handle error while processing the current entry
                    std::cerr << "Error processing entry: " << entry.path() << ": " << ex.what() << std::endl;
                }
            }
        } catch (const fs::filesystem_error& ex) {
            // Handle error while iterating over directory
            std::cerr << "Error iterating directory: " << path << ": " << ex.what() << std::endl;
        }
        return size;
    }
    
}

int main ()
{

}
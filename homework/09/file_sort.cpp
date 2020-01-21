#include "file_sort.h"

std::vector<std::string> make_chunks(std::string filename) {
    std::ifstream is (filename, std::ifstream::binary);
    std::vector<std::string> res;
    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        size_t length = is.tellg() / sizeof(uint64_t);
        is.seekg (0, is.beg);
        std::vector<uint64_t> myVec;
        int i = 0;
        
        while (!(length == 0)) {
            size_t size = std::min(length, chunk_size * sizeof(uint64_t));
            myVec.resize(size);
            is.read(reinterpret_cast<char*>(&myVec.front()),
                    myVec.size()*sizeof(uint64_t));
            std::string chunk_filename = std::to_string(i++) +
                    std::string("chunk.txt");
            res.push_back(chunk_filename);
            std::ofstream f(chunk_filename, std::ios::binary);
            if (f) {
                f.write(reinterpret_cast<char*>(myVec.data()),
                        myVec.size() * sizeof(uint64_t));
                f.close();
            }
            else {
                throw std::runtime_error("file didn't open");
            }
            length -= size;
        }
        
        is.close();
        return res;
    }
    else{
        throw std::runtime_error("file didn't open");
    }
}

void merge(std::pair<std::string, std::string> p, std::string name) {
    std::ifstream is1 (p.first, std::ifstream::binary);
    std::ifstream is2 (p.second, std::ifstream::binary);
    std::ofstream os(name, std::ios::binary);
    if (!(is1 && is2 && os)) {
        throw std::runtime_error("file didn't open");
    }
    is1.seekg (0, is1.end);
    size_t length1 = is1.tellg() / sizeof(uint64_t);
    is1.seekg (0, is1.beg);
    is2.seekg (0, is2.end);
    size_t length2 = is2.tellg() / sizeof(uint64_t);
    is2.seekg (0, is2.beg);
    
    uint64_t a;  
    is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
    uint64_t b;  
    is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
    length1--;
    length2--;
    
    while (length1 && length2) {
        if (a < b) {
            length1--;
            os.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
            is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
        }
        else {
            length2--;
            os.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
            is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
        }
    }
    while (length1) {
        length1--;
        os.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
        is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
    }
    while (length2) {
        length2--;
        os.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
        is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
    }
    is1.close();
    is2.close();
    os.close();
}

void sort_file(std::string name, std::string new_name) {
    std::ifstream is (name, std::ifstream::binary);
    std::ofstream os(new_name, std::ios::binary);
    if (!(is && os)) {
        throw std::runtime_error("file didn't open");
    }
    is.seekg (0, is.end);
    size_t length = is.tellg() / sizeof(uint64_t);
    is.seekg (0, is.beg);
    
    std::vector<uint64_t> myVec(length);
    is.read(reinterpret_cast<char*>(&myVec.front()),
                    myVec.size()*sizeof(uint64_t));
    std::sort(myVec.begin(), myVec.end());
    os.write(reinterpret_cast<char*>(myVec.data()),
                    myVec.size() * sizeof(uint64_t));
    is.close();
    os.close();
}

void sort(std::string filename, std::string to) {
    auto chunks = make_chunks(filename);
    std::vector<std::string> chunks1(chunks.size() / 2),
    chunks2(chunks.size() - chunks.size() / 2);
    std::copy(chunks.begin(), chunks.begin() + chunks1.size(), chunks1.begin());
    std::copy(chunks.begin() + chunks1.size(), chunks.end(), chunks2.begin());
    ThreadPool pool(2);
    auto sort_1_thread = [](std::vector<std::string> chunk, int counter) {
        auto s = Sorter_One_Thread(chunk, counter);
        return s.sort();
    };
    auto p1 = pool.exec(sort_1_thread, chunks1, 0);
    auto p2 = pool.exec(sort_1_thread, chunks2, 10 * chunks1.size());
    auto file1 = p1.get();
    auto file2 = p2.get();
    for (auto chunk : chunks) {
        std::remove(chunk.data());
    }
    merge(std::make_pair(file1, file2), to);
    std::remove(file1.data());
    std::remove(file2.data());
}

Sorter_One_Thread::Sorter_One_Thread(std::vector<std::string> chunks, int counter):
chunks(chunks), counter(counter) {}
    
std::string Sorter_One_Thread::sort() {
    std::queue<std::string> names;
    for (auto name: chunks) {
        auto new_name = std::to_string(counter++) + "file.txt";
        sort_file(name, new_name);
        names.push(new_name);

        files.push_back(new_name);
    }
    while (!(names.size() == 1)) {
        auto new_name = std::to_string(counter++) + "file.txt";
        auto name1 = names.front();
        names.pop();
        auto name2 = names.front();
        names.pop();

        files.push_back(new_name);

        merge(std::make_pair(name1, name2), new_name);
        std::cout << "merge " << name1 << " " << name2 << "->" << new_name << std::endl; 
        names.push(new_name);
    }
    files.pop_back();
    return names.front();
}

Sorter_One_Thread::~Sorter_One_Thread() {
   for (auto name: files) {
       std::remove(name.data());
   }
}



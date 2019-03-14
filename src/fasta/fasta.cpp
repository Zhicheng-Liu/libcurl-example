#include <memory>

#include <boost/filesystem/operations.hpp>

#include "util/file_utils.hpp"
#include "util/stream_utils.hpp"
#include "util/string_utils.hpp"
#include "fasta/fasta.hpp"
#include "util/string_constants.hpp"


class fasta::ContigFromENA {
public:
  ContigFromENA(const std::string& contigName)
  {
      contig_length = 0;
      contig_name = contigName;
      fasta_file.open(contig_name.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
  }

  ~ContigFromENA()
  {
      boost::filesystem::remove(contig_name);
  }

  void write(const char* buffer, const size_t length)
  {
      fasta_file.write(buffer, length);
      contig_length += length;
  }

  std::string read(const size_t pos, const size_t length)
  {
      if (pos >= contig_length) {
        return "";
      }

      std::string result;
      return util::read_n(fasta_file, result, length, pos);
  }

  size_t length() const {
      return contig_length;
  }

private:
  size_t contig_length;
  std::string contig_name;
  std::fstream fasta_file;
};

std::string
fasta::RemoteContig::sequence(const std::string& contig, const size_t pos, const size_t length)
{
    if (contigs.count(contig) == 0) {
        contigs[contig].reset(new fasta::ContigFromENA(contig));

        // This contig is not downloaded, try download it from ENA.
        std::string url = util::ENA_API_FASTA_URL + contig;
        std::fstream response_stream;
        response_stream.open(contig+".tmp", std::ios::in | std::ios::out | std::ios::app);
        util::open_remote(response_stream, url);
        response_stream.seekg(0, std::ios::beg);

        std::string line;
        line.reserve(1024);
        if (util::readline(response_stream, line).size() != 0) {
            if (boost::starts_with(line, ">")) {
                while (util::readline(response_stream, line).size() != 0) {
                    util::remove_end_of_line(line); // ignore linebreak at the end of line
                    contigs[contig]->write(line.c_str(), line.size());
                }
            }
        }

        response_stream.close();
        boost::filesystem::remove(contig+".tmp");
    }

    return contigs[contig]->read(pos, length);
}

size_t
fasta::RemoteContig::count(const std::string &contig) const
{
  if (contigs.find(contig) == contigs.cend()) {
    return 0;
  }

  return 1;
}

size_t
fasta::RemoteContig::len(const std::string &contig) const
{
    auto iter = contigs.find(contig);
    if (iter == contigs.cend()) {
      return 0;
    }

    return iter->second->length();
}

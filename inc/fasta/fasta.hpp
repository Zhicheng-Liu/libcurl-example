#ifndef VCF_VALIDATOR_FASTA_HPP
#define VCF_VALIDATOR_FASTA_HPP

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/iostreams/filtering_stream.hpp>

namespace fasta
{
  class IFasta
  {
  public:
    /**
     * Extract n base pairs from the contig starting from an offset
     * @param contig - the name of the contig to extract the sequence from
     * @param pos - the starting offset position
     * @param length - the number of base pairs to be extracted
     * @return the sequence string, empty if nothing can be extracted.
     */
    virtual std::string sequence(const std::string& contig, const size_t pos, const size_t length) = 0;

    /**
     * Get the number of times a contig appears in a FASTA.
     * @param contig - the name of the contig
     * @return the number of times a contig appears in a FASTA. 0 if the contig is not found.
     */
    virtual size_t count(const std::string &contig) const = 0;

    /**
     * Get the length of the sequence for a contig
     * @param contig - the name of the contig
     * @return the length of the sequence for the contig. 0 if the contig is not found.
     */
    virtual size_t len(const std::string &contig) const = 0;

    virtual ~IFasta(){}
  };

  class ContigFromENA;

  class RemoteContig : public IFasta
  {
  public:
    RemoteContig(){}
    virtual ~RemoteContig(){}

    std::string sequence(const std::string& contig, const size_t pos, const size_t length);
    size_t count(const std::string &contig) const;
    size_t len(const std::string &contig) const;

  private:
    std::unordered_map<std::string, std::shared_ptr<ContigFromENA>> contigs;
  };
}

#endif //VCF_VALIDATOR_FASTA_HPP

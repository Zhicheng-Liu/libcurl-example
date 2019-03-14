#include <fstream>
#include <string>

#include "catch/catch.hpp"

#include "fasta/fasta.hpp"

TEST_CASE("Tests for using contig downloaded from ENA", "[fasta]")
{
  SECTION("Create contig based fasta objects")
  {
      CHECK_NOTHROW(std::unique_ptr<fasta::IFasta>(new fasta::RemoteContig()));
  }

  SECTION("Download contigs and be able to extract sequences")
  {
      std::unique_ptr<fasta::IFasta> fasta(new fasta::RemoteContig());
      CHECK_NOTHROW(fasta->sequence("DQ083950", 0, 1));
      CHECK_NOTHROW(fasta->sequence("Non-existing", 0, 1));

      CHECK(fasta->count("DQ083950") == 1);
      CHECK(fasta->count("Non-existing") == 1);
      CHECK(fasta->count("Undownloaded") == 0);

      CHECK(fasta->len("DQ083950") == 174);
      CHECK(fasta->len("Non-existing") == 0);
      CHECK(fasta->len("Undownloaded") == 0);

      CHECK(fasta->sequence("DQ083950", 0, 31) == "GCCTCTGGTATGCTTCCCTGCTTGATGATTG");
      CHECK(fasta->sequence("DQ083950", 60, 31) == "ATGCTAGAAAAAACCAAGCAGTTTGTGGAGA");
      CHECK(fasta->sequence("DQ083950", 170, 31) == "GGTG");
      CHECK(fasta->sequence("DQ083950", 174, 31) == "");
      CHECK(fasta->sequence("DQ083950", 0, 0) == "");
      CHECK(fasta->sequence("Non-existing", 0, 31) == "");
  }
}

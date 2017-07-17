

#include <vector>
#include <string>
#include <string.h>
#include <slankdev/csv.h>
#include <slankdev/exception.h>

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s csvfile\n", argv[0]);
    return -1;
  }

  slankdev::csv csv;
  csv.read(argv[1]);
  if (csv.get_col() < 4) throw slankdev::exception("OKASHII");

  printf(
    "#include <stdint.h>\n"
    "#include <stddef.h>\n"
    "#include <slankdev/csv.h>\n"
    "struct oui {\n"
    "  uint32_t regist;\n"
    "  std::string org;\n"
    "};\n"
  );
  printf("oui records[] = {\n");
  size_t nb_row = csv.get_row();
  for (size_t r=1; r<nb_row; r++) {
    printf("  { 0x%s, \"%-30s\" },\n",
        csv.get(r, 1).c_str(),
        csv.get(r, 2).c_str());
  }
  printf("  { 0x%s, \"%-30s\" }\n", "FFFFFF", "end of list");
  printf("};\n");
  printf(
    "inline std::string mac2vendor(uint8_t* m)                  \n"
    "{                                                          \n"
    "  union {                                                  \n"
    "    uint8_t u8[4];                                         \n"
    "    uint32_t u32;                                          \n"
    "  } U;                                                     \n"
    "  U.u8[0] = m[2];                                          \n"
    "  U.u8[1] = m[1];                                          \n"
    "  U.u8[2] = m[0];                                          \n"
    "  U.u8[3] = 0;                                             \n"
    "                                                           \n"
    "  for (size_t i=0; ;i++) {                                 \n"
    "    if (records[i].regist == 0xFFFFFF) break;              \n"
    "    if (records[i].regist == U.u32) return records[i].org; \n"
    "  }                                                        \n"
    "  return \"not found\";                                    \n"
    "}                                                          \n"
  );
}




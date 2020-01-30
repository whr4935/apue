#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <libgen.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct ProgramHeaderTable
{
    bool is64bit;
    void* addr;
    size_t entrySize;
    size_t count;
};

struct SectionHeaderTable
{
    bool is64bit;
    void* addr;
    size_t entrySize;
    size_t count;
};

const char* sectionType2Str(Elf64_Word type)
{
    switch (type) {
    case PT_NULL:
        return "PT_NULL";
        break;

    case PT_LOAD:
        return "PT_LOAD";
        break;

    case PT_DYNAMIC:
        return "PT_DYNAMIC";
        break;

    case PT_INTERP:
        return "PT_INTERP";
        break;

    case PT_NOTE:
        return "PT_NOTE";
        break;

    case PT_SHLIB:
        return "PT_SHLIB";
        break;

    case PT_PHDR:
        return "PT_PHDR";
        break;

    case PT_LOPROC:
        return "PT_LOPROC";
        break;

    case PT_HIPROC:
        return "PT_HIPROC";
        break;

    case PT_GNU_STACK:
        return "PT_GNU_STACK";
        break;

    default:
        break;
    }

    return "";
}

void parseProgramHeaderTable(ProgramHeaderTable* t)
{
    Elf64_Phdr* h = (Elf64_Phdr*)t->addr;
    for (size_t i = 0; i < t->count; ++i, ++h) {
        printf("%lu: p_type:%d(%s), p_offset:%ld, p_filesz:%zu\n", i, h->p_type, sectionType2Str(h->p_type), h->p_offset, h->p_filesz);
    }
}

void parseSectionHeaderTable(SectionHeaderTable* t)
{

}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage %s file.\n", basename(argv[0]));
        exit(0);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("open %s failed! %d\n", argv[1], errno);
        exit(EXIT_FAILURE);
    }

    struct stat st;
    int ret = fstat(fd, &st);
    if (ret < 0) {
        printf("fstat failed! %d\n", errno);
        exit(EXIT_FAILURE);
    }

    void* addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == NULL) {
        printf("mmap failed! %d\n", errno);
        exit(EXIT_FAILURE);
    }

    Elf64_Ehdr* elf_header = (Elf64_Ehdr*)addr;
    printf("%c %c %c %c\n", elf_header->e_ident[0], elf_header->e_ident[1], elf_header->e_ident[2], elf_header->e_ident[3]);

    auto ei_class = elf_header->e_ident[EI_CLASS];
    printf("ei_class: %d, ELFCLASS32:%d, ELFCLASS64:%d\n", ei_class, ELFCLASS32, ELFCLASS64);

    auto e_type = elf_header->e_type;
    printf("e_type:%d\n", e_type);

    auto e_entry = elf_header->e_entry;
    printf("e_entry:%#lx\n", e_entry);

    printf("e_ehsize:%d\n", elf_header->e_ehsize);

    ProgramHeaderTable phTable;
    phTable.is64bit = ei_class == ELFCLASS64;
    phTable.addr = (char*)addr + elf_header->e_phoff;
    phTable.entrySize = elf_header->e_phentsize;
    phTable.count = elf_header->e_phnum;

    parseProgramHeaderTable(&phTable);

    SectionHeaderTable shTable;
    shTable.is64bit = ei_class == ELFCLASS64;
    shTable.addr = (char*)addr + elf_header->e_shoff;
    shTable.entrySize = elf_header->e_shentsize;
    shTable.count = elf_header->e_shnum;

    parseSectionHeaderTable(&shTable);

    return 0;
}

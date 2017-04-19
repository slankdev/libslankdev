
#pragma once
#include <stdint.h>
#include <string>
#include <map>
#include <pcap.h>


namespace slankdev {


enum bpf_code : uint16_t {
    NOP  = 0xffff, // original
    INT  = 0xfffe, // original
    LD   = 0x0000,
    LDX  = 0x0001,
    ST   = 0x0002,
    STX  = 0x0003,
    W    = 0x0000,
    H    = 0x0008,
    B    = 0x0010,
    IMM  = 0x0000,
    ABS  = 0x0020,
    IND  = 0x0040,
    MEM  = 0x0060,
    LEN  = 0x0080,
    MSH  = 0x00a0,
    ALU  = 0x0004,
    JMP  = 0x0005,
    ADD  = 0x0000,
    SUB  = 0x0010,
    MUL  = 0x0020,
    DIV  = 0x0030,
    OR   = 0x0040,
    AND  = 0x0050,
    LSH  = 0x0060,
    RSH  = 0x0070,
    NEG  = 0x0080,
    JA   = 0x0000,
    JEQ  = 0x0010,
    JGT  = 0x0020,
    JGE  = 0x0030,
    JSET = 0x0040,
    K    = 0x0000,
    X    = 0x0008,
    RET  = 0x0006,
    A    = 0x0010,
    MISC = 0x0007,
    TAX  = 0x0000,
    TXA  = 0x0080,
};


const char* op2str(uint16_t op)
{
    switch (op) {
        case NOP       : return "NOP       "; //original
        case INT       : return "INT       "; //original
        case RET|K     : return "RET|K     ";
        case RET|A     : return "RET|A     ";
        case LD|W|ABS  : return "LD|W|ABS  ";
        case LD|H|ABS  : return "LD|H|ABS  ";
        case LD|B|ABS  : return "LD|B|ABS  ";
        case LD|W|LEN  : return "LD|W|LEN  ";
        case LDX|W|LEN : return "LDX|W|LEN ";
        case LD|W|IND  : return "LD|W|IND  ";
        case LD|H|IND  : return "LD|H|IND  ";
        case LD|B|IND  : return "LD|B|IND  ";
        case LDX|MSH|B : return "LDX|MSH|B ";
        case LD|IMM    : return "LD|IMM    ";
        case LDX|IMM   : return "LDX|IMM   ";
        case LD|MEM    : return "LD|MEM    ";
        case LDX|MEM   : return "LDX|MEM   ";
        case ST        : return "ST        ";
        case STX       : return "STX       ";
        case JMP|JA    : return "JMP|JA    ";
        case JMP|JGT|K : return "JMP|JGT|K ";
        case JMP|JGE|K : return "JMP|JGE|K ";
        case JMP|JEQ|K : return "JMP|JEQ|K ";
        case JMP|JSET|K: return "JMP|JSET|K";
        case JMP|JGT|X : return "JMP|JGT|X ";
        case JMP|JGE|X : return "JMP|JGE|X ";
        case JMP|JEQ|X : return "JMP|JEQ|X ";
        case JMP|JSET|X: return "JMP|JSET|X";
        case ALU|ADD|X : return "ALU|ADD|X ";
        case ALU|SUB|X : return "ALU|SUB|X ";
        case ALU|MUL|X : return "ALU|MUL|X ";
        case ALU|DIV|X : return "ALU|DIV|X ";
        case ALU|AND|X : return "ALU|AND|X ";
        case ALU|OR|X  : return "ALU|OR|X  ";
        case ALU|LSH|X : return "ALU|LSH|X ";
        case ALU|RSH|X : return "ALU|RSH|X ";
        case ALU|ADD|K : return "ALU|ADD|K ";
        case ALU|SUB|K : return "ALU|SUB|K ";
        case ALU|MUL|K : return "ALU|MUL|K ";
        case ALU|DIV|K : return "ALU|DIV|K ";
        case ALU|AND|K : return "ALU|AND|K ";
        case ALU|OR|K  : return "ALU|OR|K  ";
        case ALU|LSH|K : return "ALU|LSH|K ";
        case ALU|RSH|K : return "ALU|RSH|K ";
        case ALU|NEG   : return "ALU|NEG   ";
        case MISC|TAX  : return "MISC|TAX  ";
        case MISC|TXA  : return "MISC|TXA  ";
        default        : return "UNKNOWN   ";
    }
}


struct dict_element {
  uint16_t hex;
  std::string mn  ;
  std::string op  ;
  std::string dsc ;
  std::string ck  ;
  std::string cmnt;
  std::string fmt ;
};
using dict = std::map<uint16_t, dict_element>;


void read_dict(dict& m, dict_element dic[], size_t dic_len)
{
  for (size_t i=0; i<dic_len; i++) {
    dict_element v(dic[i]);
    m.insert(std::make_pair(v.hex, v));
  }
}



dict_element raw_dict_bpf[] = {
    { RET|K     ,"ret" , "RET|K"       , "return k"         , "", "", "ret  #%u"},
    { RET|A     ,"ret" , "RET|A"       , "return A"         , "", "", "ret"    },

    {
        LD|W|ABS
        , "ld"
        , "LD|W|ABS"
        , "A=ntohl(*(i32*)(p+k))"
        , "[k>buflen]or[size_i32>buflen-k]"
        , "SIGSEGV"
        , "ld   [%u]"
    },

    {
        LD|H|ABS
        , "ldh"
        , "LD|H|ABS"
        , "A=EXTRACT_SHORT(&p[k])"
        , "[k>buflen]or[size_i16>buflen-k]"
        , "SIGSEGV"
        , "ldh  [%u]"
    },

    {
        LD|B|ABS
        , "ldb"
        , "LD|B|ABS"
        , "A=p[k]"
        , "k>=buflen"
        , "SIGSEGV"
        , "ldb  [%u]"
    },

    { LD|W|LEN  ,"ld"  , "LD|W|LEN"    , "A=wirelen"        , "", "", "ld   #pktlen"},
    { LDX|W|LEN ,"ldx" , "LDX|W|LEN"   , "X=wirelen"        , "", "", "(n/a)"     },

    {
        LD|W|IND
        , "ld"
        , "LD|W|IND"
        , "A=ntohl(*(i32*)(p+k))"
        , "[k>buflen] or [X>buflen-k] or [size_i32>buflen-k]"
        , "SIGSEGV"
        , "ld   [%u]"
    },

    {
        LD|H|IND
        , "ldh"
        , "LD|H|IND"
        , "A=EXTRACT_SHORT(&p[k])"
        , "[X>buflen] or [k>buflen-X]or[size_i16>buflen-k]"
        , "SIGSEGV"
        , "ldh  [%u]"
    },

    {
        LD|B|IND
        , "ldb"
        , "LD|B|IND"
        , "A=p[k]"
        , "[k>=buflen]or[X>=buflen-k]"
        , "SIGSEGV"
        , "ldb  [%u]"
    },

    {
        LDX|MSH|B
        , "ldxb"
        , "LDX|MSH|B"
        , "X=(p[k]&0xf)<<2"
        , "k>=buflen"
        , "SIGSEGV"
        , "ldxb 4*([%u]&0xf)"
    },

    { LD|IMM    ,"ld"  , "LD|IMM"      , "A=k"              , "", "", "ld   #0x%-8x" },
    { LDX|IMM   ,"ldx" , "LDX|IMM"     , "X=k"              , "", "", "ldx  #0x%-8x" },
    { LD|MEM    ,"ld"  , "LD|MEM"      , "A=mem[k]"         , "", "", "ld   M[%u]" },
    { LDX|MEM   ,"ldx" , "LDX|MEM"     , "X=mem[k]"         , "", "", "ldx  M[%u]" },
    { ST        ,"st"  , "ST"          , "mem[k]=A"         , "", "", "st   M[%u]" },
    { STX       ,"stx" , "STX"         , "mem[k]=X"         , "", "", "stx  M[%u]" },

    { JMP|JA    ,"ja"  , "JMP|JA"      , "pc+=k"            , "", "", "ja   %u"    },

    {
        JMP|JGT|K
        ,"jgt"
        , "JMP|JGT|K"
        , "pc+=(A>k)?jt:jf"
        , ""
        , ""
        , "jgt  #0x%-8x jt %-3u  jf %-3u"
    },

    {
        JMP|JGE|K
        ,"jge"
        , "JMP|JGE|K"
        , "pc+=(A>=k)?jt:jf"
        , ""
        , ""
        , "jge  #0x%-8x jt %-3u  jf %-3u"
    },

    {
        JMP|JEQ|K
        ,"jeq"
        , "JMP|JEQ|K"
        , "pc+=(A==k)?jt:jf"
        , ""
        , ""
        , "jeq  #0x%-8x jt %-3u  jf %-3u"
    },

    {
        JMP|JSET|K
        ,"jset"
        , "JMP|JSET|K"
        , "pc+=(A&k)?jt:jf"
        , ""
        , ""
        , "jset #0x%-8x jt %-3u  jf %-3u"
    },

    {
        JMP|JGT|X
        ,"jgt"
        , "JMP|JGT|X"
        , "pc+=(A>X)?jt:jf"
        , ""
        , ""
        , "jgt  x jt %-3u  jf %-3u"
    },

    {
        JMP|JGE|X
        ,"jge"
        , "JMP|JGE|X"
        , "pc+=(A>=X)?jt:jf"
        , ""
        , ""
        , "jge  x jt %-3u  jt %-3u"
    },

    {
        JMP|JEQ|X
        ,"jeq"
        , "JMP|JEQ|X"
        , "pc+=(A==X)?jt:jf"
        , ""
        , ""
        , "jeq  x jt %-3u  jt %-3u"
    },

    {
        JMP|JSET|X,
        "jset", "JMP|JSET|X"
        , "pc+=(A&X)?jt:jf"
        , ""
        , ""
        , "jset x jt %-3u  jt %-3u"
    },

    { ALU|ADD|X ,"add" , "ALU|ADD|X"   , "A+=X"             , "", "", "add  x"},
    { ALU|SUB|X ,"sub" , "ALU|SUB|X"   , "A-=X"             , "", "", "sub  x"},
    { ALU|MUL|X ,"mul" , "ALU|MUL|X"   , "A*=X"             , "", "", "mul  x"},

    {
        ALU|DIV|X
        ,"div"
        , "ALU|DIV|X"
        , "A/=X"
        , "div 0"
        , "SIGFPE"
        , "div  x"
    },

    { ALU|AND|X ,"and" , "ALU|AND|X"   , "A&=X"             , "", "", "and  x"},
    { ALU|OR|X  ,"or"  , "ALU|OR|X"    , "A|=X"             , "", "", "or   x"},
    { ALU|LSH|X ,"lsh" , "ALU|LSH|X"   , "A<<=X"            , "", "", "lsh  x"},
    { ALU|RSH|X ,"rsh" , "ALU|RSH|X"   , "A>>=X"            , "", "", "rsh  x"},

    { ALU|ADD|K ,"add" , "ALU|ADD|K"   , "A+=k"             , "", "", "add  #%u"},
    { ALU|SUB|K ,"sub" , "ALU|SUB|K"   , "A-=k"             , "", "", "sub  #%u"},
    { ALU|MUL|K ,"mul" , "ALU|MUL|K"   , "A*=k"             , "", "", "mul  #%u"},

    {
        ALU|DIV|K
        ,"div"
        , "ALU|DIV|K"
        , "A/=k"
        , "div 0"
        , "SIGFPE"
        , "div  #%u"
    },

    { ALU|AND|K ,"and" , "ALU|AND|K"   , "A&=k"             , "", "", "and  #0x%-8x" },
    { ALU|OR|K  ,"or"  , "ALU|OR|K"    , "A|=k"             , "", "", "or   #0x%-8x" },
    { ALU|LSH|K ,"lsh" , "ALU|LSH|K"   , "A<<=k"            , "", "", "lsh  #%u"   },
    { ALU|RSH|K ,"rsh" , "ALU|RSH|K"   , "A>>=k"            , "", "", "rsh  #%u"   },
    { ALU|NEG   ,"neg" , "ALU|NEG"     , "A=-A"             , "", "", "neg"},
    { MISC|TAX  ,"tax" , "MISC|TAX"    , "X=A"              , "", "", "tax"},
    { MISC|TXA  ,"txa" , "MISC|TXA"    , "A=X"              , "", "", "txa"},
    { NOP       ,"nop" , "NOP"         , "no operation"     , "", "", "nop"    },
    { INT       ,"int" , "INT"         , "interrupt"        , "", "", "int  %u" },
};



inline std::string dissasemble_instruction(const bpf_insn* inst)
{
  dict dict;
  read_dict(dict, raw_dict_bpf, sizeof(raw_dict_bpf)/sizeof(raw_dict_bpf[0]));

  uint16_t code = inst->code;
  uint8_t  jt   = inst->jt  ;
  uint8_t  jf   = inst->jf  ;
  uint32_t k    = inst->k   ;

  char str[1000];
  const char* fmt = dict[code].fmt.c_str();
  switch (code) {
      case NOP       : sprintf(str, fmt   ); break;
      case RET|K     : sprintf(str, fmt, k); break;
      case RET|A     : sprintf(str, fmt   ); break;
      case LD|W|ABS  : sprintf(str, fmt, k); break;
      case LD|H|ABS  : sprintf(str, fmt, k); break;
      case LD|B|ABS  : sprintf(str, fmt, k); break;
      case LD|W|LEN  : sprintf(str, fmt   ); break;
      case LDX|W|LEN : sprintf(str, fmt   ); break;
      case LD|W|IND  : sprintf(str, fmt, k); break;
      case LD|H|IND  : sprintf(str, fmt, k); break;
      case LD|B|IND  : sprintf(str, fmt, k); break;
      case LDX|MSH|B : sprintf(str, fmt, k); break;
      case LD|IMM    : sprintf(str, fmt, k); break;
      case LDX|IMM   : sprintf(str, fmt, k); break;
      case LD|MEM    : sprintf(str, fmt, k); break;
      case LDX|MEM   : sprintf(str, fmt, k); break;
      case ST        : sprintf(str, fmt, k); break;
      case STX       : sprintf(str, fmt, k); break;
      case JMP|JA    : sprintf(str, fmt, k); break;
      case JMP|JGT|K : sprintf(str, fmt, k, jt+1, jf+1); break;
      case JMP|JGE|K : sprintf(str, fmt, k, jt+1, jf+1); break;
      case JMP|JEQ|K : sprintf(str, fmt, k, jt+1, jf+1); break;
      case JMP|JSET|K: sprintf(str, fmt, k, jt+1, jf+1); break;
      case JMP|JGT|X : sprintf(str, fmt,    jt+1, jf+1); break;
      case JMP|JGE|X : sprintf(str, fmt,    jt+1, jf+1); break;
      case JMP|JEQ|X : sprintf(str, fmt,    jt+1, jf+1); break;
      case JMP|JSET|X: sprintf(str, fmt,    jt+1, jf+1); break;
      case ALU|ADD|X : sprintf(str, fmt   ); break;
      case ALU|SUB|X : sprintf(str, fmt   ); break;
      case ALU|MUL|X : sprintf(str, fmt   ); break;
      case ALU|DIV|X : sprintf(str, fmt   ); break;
      case ALU|AND|X : sprintf(str, fmt   ); break;
      case ALU|OR|X  : sprintf(str, fmt   ); break;
      case ALU|LSH|X : sprintf(str, fmt   ); break;
      case ALU|RSH|X : sprintf(str, fmt   ); break;
      case ALU|ADD|K : sprintf(str, fmt, k); break;
      case ALU|SUB|K : sprintf(str, fmt, k); break;
      case ALU|MUL|K : sprintf(str, fmt, k); break;
      case ALU|DIV|K : sprintf(str, fmt, k); break;
      case ALU|AND|K : sprintf(str, fmt, k); break;
      case ALU|OR|K  : sprintf(str, fmt, k); break;
      case ALU|LSH|K : sprintf(str, fmt, k); break;
      case ALU|RSH|K : sprintf(str, fmt, k); break;
      case ALU|NEG   : sprintf(str, fmt   ); break;
      case MISC|TAX  : sprintf(str, fmt   ); break;
      case MISC|TXA  : sprintf(str, fmt   ); break;
      default        : sprintf(str, "unknown"); break;
  }
  return str;
}



void dissas(const struct bpf_insn* inst, size_t len)
{
  for (size_t i=0; i<len; i++) {
    printf("(%03zd) %04x %02x %02x %08x      %s \n",
        i,
        inst[i].code,
        inst[i].jt,
        inst[i].jf,
        inst[i].k,
        dissasemble_instruction(&inst[i]).c_str()
    );
  }
}



} /* namespace slankdev */




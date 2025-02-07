#include "cpuid.h"

#include <kernel/vga_textmode/vga_textmode.h>
#include <stdint.h>

const char *get_vendor_from_model(uint32_t model) {
  switch (model) {
  case signature_AMD_ebx:
  case signature_AMD_ecx:
  case signature_AMD_edx:
    return "AMD";
  case signature_CENTAUR_ebx:
  case signature_CENTAUR_ecx:
  case signature_CENTAUR_edx:
    return "Centaur";
  case signature_CYRIX_ebx:
  case signature_CYRIX_ecx:
  case signature_CYRIX_edx:
    return "Cyrix";
  case signature_INTEL_ebx:
  case signature_INTEL_ecx:
  case signature_INTEL_edx:
    return "Intel";
  case signature_TM1_ebx:
  case signature_TM1_ecx:
  case signature_TM1_edx:
  case signature_TM2_ecx:
  case signature_TM2_edx:
    return "Transmeta";
  case signature_NSC_ebx:
  case signature_NSC_ecx:
  case signature_NSC_edx:
    return "NSC";
  case signature_NEXGEN_ebx:
  case signature_NEXGEN_ecx:
  case signature_NEXGEN_edx:
    return "NexGen";
  case signature_RISE_ebx:
    return "Rise";
  case signature_SIS_ebx:
    return "SiS";
  case signature_UMC_ebx:
    return "UMC";
  case signature_VIA_ebx:
    return "VIA";
  default:
    return "Unknown";
  }
}

void check_cpuid_features() {
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
    vga_printf("CPUID instruction not supported.\n");
    return;
  }

  if (check_apic()) {
    vga_printf("APIC supported\n");
  } else {
    vga_printf("APIC not supported\n");
  }

  uint32_t model = get_model();
  vga_printf("CPU Vendor: %s\n", get_vendor_from_model(model));
  vga_printf("CPU Model: %x\n", model);

  vga_printf("CPU Features:\n");
  if (ecx & bit_SSE3)
    vga_printf("  - SSE3\n");
  if (ecx & bit_PCLMUL)
    vga_printf("  - PCLMULQDQ\n");
  if (ecx & bit_SSSE3)
    vga_printf("  - SSSE3\n");
  if (ecx & bit_FMA)
    vga_printf("  - FMA\n");
  if (ecx & bit_SSE4_1)
    vga_printf("  - SSE4.1\n");
  if (ecx & bit_SSE4_2)
    vga_printf("  - SSE4.2\n");
  if (ecx & bit_AES)
    vga_printf("  - AES\n");
  if (ecx & bit_AVX)
    vga_printf("  - AVX\n");
  if (ecx & bit_F16C)
    vga_printf("  - F16C\n");
  if (ecx & bit_RDRND)
    vga_printf("  - RDRAND\n");

  if (edx & bit_MMX)
    vga_printf("  - MMX\n");
  if (edx & bit_SSE)
    vga_printf("  - SSE\n");
  if (edx & bit_SSE2)
    vga_printf("  - SSE2\n");

  if (!__get_cpuid(7, &eax, &ebx, &ecx, &edx))
    return;

  if (ebx & bit_AVX2)
    vga_printf("  - AVX2\n");
  if (ebx & bit_BMI)
    vga_printf("  - BMI1\n");
  if (ebx & bit_BMI2)
    vga_printf("  - BMI2\n");
  if (ebx & bit_SHA)
    vga_printf("  - SHA\n");
  if (ebx & bit_RDSEED)
    vga_printf("  - RDSEED\n");
  if (ebx & bit_ADX)
    vga_printf("  - ADX\n");
  if (ebx & bit_AVX512F)
    vga_printf("  - AVX-512 Foundation\n");
  if (ebx & bit_AVX512DQ)
    vga_printf("  - AVX-512 DQ\n");
  if (ebx & bit_AVX512BW)
    vga_printf("  - AVX-512 BW\n");
  if (ebx & bit_AVX512VL)
    vga_printf("  - AVX-512 VL\n");

  if (ecx & bit_AVX512VBMI)
    vga_printf("  - AVX-512 VBMI\n");
  if (ecx & bit_GFNI)
    vga_printf("  - GFNI\n");
  if (ecx & bit_VAES)
    vga_printf("  - VAES\n");
  if (ecx & bit_VPCLMULQDQ)
    vga_printf("  - VPCLMULQDQ\n");
  if (ecx & bit_AVX512VNNI)
    vga_printf("  - AVX-512 VNNI\n");
  if (ecx & bit_AVX512BITALG)
    vga_printf("  - AVX-512 BITALG\n");
  if (ecx & bit_AVX512VPOPCNTDQ)
    vga_printf("  - AVX-512 VPOPCNTDQ\n");
}

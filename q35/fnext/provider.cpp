#include "q35/ns.hpp"
#include <gufo/ext/generated/qwen38_flash_next_constants.hpp>
#include <gufo/ext/registry.hpp>
#include <gufo/ext/text.hpp>

#include "q35/fnext/probe.hpp"
#include "q35/fnext/runner.hpp"

namespace strycast::models::qwen38_flash_next {

#if defined(ENGINE_ENABLE_HIP)
int RunQwen38FlashNextBenchmark(
    const ext::TextBenchOptions& opt,
    const std::shared_ptr<const core::GgufReader>& reader,
    std::chrono::steady_clock::time_point model_load_start);

int RunFlashNextPrompt(const ext::TextPromptOptions& opt,
                       const core::GgufReader& reader,
                       std::chrono::steady_clock::time_point model_load_start);

int RunFlashNextChat(const ext::TextPromptOptions& opt,
                     const core::GgufReader& reader,
                     std::chrono::steady_clock::time_point model_load_start);
#endif

}  // namespace strycast::models::qwen38_flash_next

void qwen38_flash_next_provider_register(strycast::ext::Registry& registry) {
  registry.AddProvider(strycast::ext::generated::extension_qwen38_flash_next::
                           kProvider_flash_next_q4);
#if defined(ENGINE_ENABLE_HIP)
  registry.AddTextProvider({
      .id = "flash-next-q4",
      .probe = &strycast::models::qwen38_flash_next::ProbeFlashNext,
      .probe_gguf = &strycast::models::qwen38_flash_next::ProbeFlashNextGguf,
      .create = &strycast::models::qwen38_flash_next::CreateQwenFlashNextTextRunner,
      .bench_driver =
          &strycast::models::qwen38_flash_next::RunQwen38FlashNextBenchmark,
      .prompt_driver = &strycast::models::qwen38_flash_next::RunFlashNextPrompt,
      .chat_driver = &strycast::models::qwen38_flash_next::RunFlashNextChat,
      .sampling_defaults =
          &strycast::models::qwen38_flash_next::FlashNextSamplingDefaults,
      .speculative_defaults = {{.backend = "mtp",
                                .max_draft_tokens = 7,
                                .min_draft_tokens = 1}},
  });
#endif
}

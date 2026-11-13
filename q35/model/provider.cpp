#include "q35/ns.hpp"
#include <gufo/ext/generated/strycast_qwen35moe_constants.hpp>
#include <gufo/ext/registry.hpp>
#include <gufo/ext/text.hpp>

#include "q35/model/probe.hpp"
#include "q35/model/runner.hpp"

namespace strycast::models::qwen {

#if defined(ENGINE_ENABLE_HIP)
int RunQwenBenchmark(const ext::TextBenchOptions& opt,
                     const std::shared_ptr<const core::GgufReader>& reader,
                     std::chrono::steady_clock::time_point model_load_start);

int RunQwenPrompt(const ext::TextPromptOptions& opt,
                  const core::GgufReader& reader,
                  std::chrono::steady_clock::time_point model_load_start);

int RunQwenChat(const ext::TextPromptOptions& opt,
                const core::GgufReader& reader,
                std::chrono::steady_clock::time_point model_load_start);
#endif

}  // namespace strycast::models::qwen

void strycast_qwen35moe_provider_register(strycast::ext::Registry& registry) {
  registry.AddProvider(
      strycast::ext::generated::extension_strycast_qwen35moe::kProvider_qwen35moe);
#if defined(ENGINE_ENABLE_HIP)
  registry.AddTextProvider({
      .id = "qwen35moe",
      .probe = &strycast::models::qwen::ProbeQwen,
      .probe_gguf = &strycast::models::qwen::ProbeQwenGguf,
      .create = &strycast::models::qwen::CreateQwenTextRunner,
      .bench_driver = &strycast::models::qwen::RunQwenBenchmark,
      .prompt_driver = &strycast::models::qwen::RunQwenPrompt,
      .chat_driver = &strycast::models::qwen::RunQwenChat,
      .sampling_defaults = &strycast::models::qwen::QwenSamplingDefaults,
      .speculative_defaults = {{.backend = "dflash2",
                                .max_draft_tokens = 7,
                                .min_draft_tokens = 1,
                                .draft_policy = "adaptive",
                                .draft_policies = {"fixed", "adaptive"}}},
  });
#endif
}

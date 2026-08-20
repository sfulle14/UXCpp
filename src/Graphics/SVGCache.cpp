/* 
 * MIT License
 * Copyright (c) 2026 UXCpp Contributors
 */

#include <UXCpp/Graphics/SVGAsset.h>
#include <map>
#include <memory>

namespace uxcpp::graphics {

std::map<std::string, std::shared_ptr<SVGAsset>> SVGPipeline::m_cache;

} // namespace uxcpp::graphics
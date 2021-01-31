#include "extensions/filters/http/waf/config.h"

#include "envoy/extensions/filters/http/waf/v3/waf.pb.h"
#include "envoy/extensions/filters/http/waf/v3/waf.pb.validate.h"
#include "envoy/registry/registry.h"

#include "extensions/filters/http/waf/waf_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace WAF {

Http::FilterFactoryCb WAFFilterFactory::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::waf::v3::WAF& proto_config,
    const std::string& /*stats_prefix*/, Server::Configuration::FactoryContext& /*context*/) {
  WAFFilterConfigSharedPtr config =
      std::make_shared<WAFFilterConfig>(WAFFilterConfig(proto_config));

  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    auto filter = new WAFFilter(config);
    callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
  };
}

Router::RouteSpecificFilterConfigConstSharedPtr
WAFFilterFactory::createRouteSpecificFilterConfigTyped(
    const envoy::extensions::filters::http::waf::v3::WAFPerRoute& proto_config,
    Server::Configuration::ServerFactoryContext& context, ProtobufMessage::ValidationVisitor&) {
  return std::make_shared<WAFFilterConfigPerRoute>(proto_config, context);
}

/**
 * Static registration for the waf filter. @see NamedHttpFilterConfigFactory.
 */
REGISTER_FACTORY(WAFFilterFactory,
                 Server::Configuration::NamedHttpFilterConfigFactory){"envoy.waf"};

} // namespace WAF
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy

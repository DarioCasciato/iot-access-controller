// =============================================================================
// Template-Project | Access
// =============================================================================

#include <Arduino.h>


namespace Access
{
    enum class ResponseStatus : uint8_t
    {
        Ok,
        Denied,
        ConnectionFailed,
        ParseError
    };


    /// @brief Check if the given UID has access by sending a request to the server
    /// @param uid The UID to check
    /// @return True if the UID has access, false otherwise
    ///
    ResponseStatus request(const String& uid);

} // namespace Access
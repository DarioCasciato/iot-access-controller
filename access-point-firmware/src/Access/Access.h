// =============================================================================
// Template-Project | Access
// =============================================================================

#include <Arduino.h>


namespace Access
{
    enum class ResponseStatus : uint8_t
    {
        Ok = 0,
        Denied = 1,
        ConnectionFailed = 2,
        ParseError = 3
    };


    /// @brief Check if the given UID has access by sending a request to the server
    /// @param uid The UID to check
    /// @return True if the UID has access, false otherwise
    ///
    ResponseStatus request(const String& uid);

} // namespace Access
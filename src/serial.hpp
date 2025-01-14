#include "nucleof411re.hpp"
#include "ring_buffer.hpp"

/*
 * @brief Serial device using USART and interrupts
 */
class SerialDev {
  public:
    /*******************************************************************************
     * @brief Create and the serial device.
     *
     * Precondition: peripheral must be accessible (bus clocks enabled...)
     *
     * IRQ line must be enabled for TX/RX and the handler must call irq_handler
     * for RX/TX to work correctly.
     *
     * @param periph USART peripheral to use
     * @param irq IRQ line of the used @p periph
     ******************************************************************************/
    explicit SerialDev(volatile USART *periph);

    SerialDev(const SerialDev &) = delete;
    SerialDev(SerialDev &&) = delete;

    SerialDev &
    operator=(const SerialDev &) = delete;
    SerialDev &
    operator=(SerialDev &&) = delete;

    /*******************************************************************************
     * @brief Does NOT deinitialize the peripheral currently.
     *
     * TODO
     ******************************************************************************/
    ~SerialDev() = default;

    /*******************************************************************************
     * @brief Process pending interrupts for the USART peripheral.
     *
     * Should be called from the interrupt handler.
     ******************************************************************************/
    void
    irq_handler();

    /*******************************************************************************
     * @brief Send data over the serial line.
     *
     * Blocks if the internal TX buffer is full, the sending itself is
     *non-blocking, done via irq_handler().
     *
     * @param msg Null-terminated message.
     ******************************************************************************/
    void
    send(const char *msg);

    /*******************************************************************************
     * @brief Sends a single character.
     *
     * See send() for details.
     ******************************************************************************/
    void
    send(char c);

    /*******************************************************************************
     * @brief Receive up to @p n bytes from the serial line.
     *
     * Blocking, wait until at least one bytes is received, then it returns
     * after all @p n bytes were received or if the line becomes idle.
     *
     * @param dst Valid pointer to at least @p n bytes of storage.
     * @param n Maximum number of bytes to receive.
     * @return Number of bytes written to @p dst, in range [1,n].
     ******************************************************************************/
    size_t
    recv(char *dst, size_t n);

    /*
     * @brief Status flags of the serial line.
     *
     * Not exposed currently.
     */
    enum class StatusFlags : std::uint32_t {
        None = 0,
        IdleLine = 1U << 0,
        Overrun = 1U << 1,
    };

  private:
    /*! Pointer to the peripheral registers, always assumed to be valid. */
    volatile USART *m_usart;
    /*! Current status flags, updated by irq_handler(). */
    std::atomic<StatusFlags> m_status_flags;
    /*! Outgoing buffer, filled by send(), emptied by IRQ. */
    RingBuffer<unsigned char, 64> m_tx;
    /*! Incoming buffer, filled by IRQ, emptied by recv(). */
    RingBuffer<unsigned char, 64> m_rx;
};

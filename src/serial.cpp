#include "serial.hpp"

namespace {
bool
rx_empty(uint32_t status_reg) {
    return (status_reg & (1 << 5)) == 0;
}
bool
rx_idle(uint32_t status_reg) {
    return (status_reg & (1 << 4)) != 0;
}

bool
tx_empty(uint32_t status_reg) {
    return (status_reg & (1 << 7)) != 0;
}
bool
tx_complete(uint32_t status_reg) {
    return (status_reg & (1 << 6)) != 0;
}

inline SerialDev::StatusFlags
operator|(SerialDev::StatusFlags a, SerialDev::StatusFlags b) {
    using T = std::underlying_type_t<SerialDev::StatusFlags>;
    return static_cast<SerialDev::StatusFlags>(static_cast<T>(a) |
                                               static_cast<T>(b));
}
inline SerialDev::StatusFlags
operator&(SerialDev::StatusFlags a, SerialDev::StatusFlags b) {
    using T = std::underlying_type_t<SerialDev::StatusFlags>;
    return static_cast<SerialDev::StatusFlags>(static_cast<T>(a) &
                                               static_cast<T>(b));
}
inline SerialDev::StatusFlags
operator~(SerialDev::StatusFlags a) {
    using T = std::underlying_type_t<SerialDev::StatusFlags>;
    return static_cast<SerialDev::StatusFlags>(~static_cast<T>(a));
}
inline SerialDev::StatusFlags &
operator|=(SerialDev::StatusFlags &a, SerialDev::StatusFlags b) {
    return a = a | b;
}
inline SerialDev::StatusFlags &
operator&=(SerialDev::StatusFlags &a, SerialDev::StatusFlags b) {
    return a = a & b;
}

} // namespace

SerialDev::SerialDev(volatile USART *periph) : m_usart(periph) {
    // Setup baudrate
    // 104.166666
    // 104=0x67
    // .16 << 4 = 2.56 ~ 3 = 0x3
    m_usart->brr = 0x683;
    // Enable UART, disable all interrupts, parity...
    m_usart->cr1 = (0x1 << 13);

    // Set 1 stop bits, disable the rest
    m_usart->cr2 = 0;
    // Disable extensions
    m_usart->cr3 = 0;

    // Enable RXNEIE,IDLEIE interrupt
    m_usart->cr1 |= (11 << 4);

    // Enable RX,TX
    m_usart->cr1 |= (11 << 2);
}

void
SerialDev::irq_handler() {
    auto status = m_usart->sr;
    auto status_flags = m_status_flags.load();
    // Handle sending
    if (tx_empty(status)) {
        if (!m_tx.is_empty()) {
            // Send the next byte
            m_usart->dr = m_tx.pop_front();
        } else if (tx_complete(status)) {
            // Disable TXEIE, TCIE
            m_usart->cr1 &= ~(0b11U << 6);
        }
    }
    // Handle receiving
    if (!rx_empty(status)) {
        auto b = static_cast<unsigned char>(m_usart->dr & 0xFF);
        if (!m_rx.is_full()) {
            m_rx.push_back(b);
        } else {
            status_flags |= StatusFlags::Overrun;
        }
    }

    if (rx_idle(status) && rx_empty(status)) {
        status_flags |= StatusFlags::IdleLine;
        // Disable IDLEIE
        m_usart->cr1 &= ~(0b1U << 4);
    } else {
        status_flags &= ~StatusFlags::IdleLine;
        // Watch for IDLE flag
        m_usart->cr1 |= 0b1U << 4;
    }

    if (status & (1 << 3)) {
        status_flags |= StatusFlags::Overrun;
    }

    m_status_flags.store(status_flags);
}

void
SerialDev::send(const char *msg) {
    while (m_tx.is_full())
        wait_for_interrupt();
    // Push first char, then enable interrupts, then push the rest
    if (*msg) {
        m_tx.push_back(*msg++);
        // Enable TXEIE, TCIE
        m_usart->cr1 |= (0b11 << 6);
    }
    while (*msg) {
        while (m_tx.is_full())
            wait_for_interrupt();
        m_tx.push_back(*msg++);
    }
}

void
SerialDev::send(char c) {
    send(std::array{c, '\0'}.data());
}

size_t
SerialDev::recv(char *dst, size_t n) {
    size_t i = 0;

    // Wait for at least one byte, after that exit early if line becomes idle.
    while (i < n && (i == 0 || !m_rx.is_empty() ||
                     (m_status_flags.load() & StatusFlags::IdleLine) ==
                         StatusFlags::None)) {

        if (!m_rx.is_empty()) {
            dst[i++] = m_rx.pop_front();
        } else {
            wait_for_interrupt();
        }
    }
    return i;
}

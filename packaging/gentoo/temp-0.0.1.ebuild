EAPI=8

DESCRIPTION="Hardware temperature monitor for Linux"
HOMEPAGE="https://github.com/danko1122/temp-monitor"
SRC_URI="https://github.com/danko1122/temp-monitor/archive/v${PV}.tar.gz -> ${P}.tar.gz"

LICENSE="MIT"
SLOT="0"
KEYWORDS="~amd64 ~x86 ~arm64"

DEPEND=""
RDEPEND="${DEPEND}"

src_compile() {
    emake
}

src_install() {
    dobin bin/temp
    dodoc README.md USAGE.md
}

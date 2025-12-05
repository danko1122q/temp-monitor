# Copyright 2024 Danko
# Distributed under the terms of the MIT License

EAPI=8

DESCRIPTION="Real-time hardware temperature monitor for Linux"
HOMEPAGE="https://github.com/danko1122/temp-monitor"
SRC_URI="https://github.com/danko1122/temp-monitor/archive/v${PV}.tar.gz -> ${P}.tar.gz"

LICENSE="MIT"
SLOT="0"
KEYWORDS="~amd64 ~x86 ~arm64"

DEPEND=""
RDEPEND="${DEPEND}"
BDEPEND="sys-devel/gcc"

src_compile() {
    emake || die "Build failed"
}

src_install() {
    dobin bin/temp
    dodoc README.md USAGE.md CHANGELOG.md
    insinto /usr/share/licenses/${PN}
    doins LICENSE
}

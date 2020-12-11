# Copyright (c) Microsoft Corporation
# All rights reserved.
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
# to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


FROM ubuntu:18.04

ENV \
  GRAFANA_VERSION=6.7.4 \
  GF_PLUGIN_DIR=/grafana-plugins \
  GF_PATHS_LOGS=/var/log/grafana \
  GF_PATHS_DATA=/var/lib/grafana \
  UPGRADEALL=true


RUN \
  apt-get update && \
  apt-get -y --force-yes --no-install-recommends install libfontconfig curl git ca-certificates && \
  curl https://dl.grafana.com/oss/release/grafana_${GRAFANA_VERSION}_amd64.deb > /tmp/grafana.deb && \
  dpkg -i /tmp/grafana.deb && \
  rm -f /tmp/grafana.deb && \
  ### branding && \
  apt-get autoremove -y --force-yes && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/*


COPY src/run-grafana.sh /usr/local/bin
COPY src/dashboards/* /usr/local/grafana/dashboards/

RUN git clone https://github.com/tghfly/grafana-chinese.git /tmp/grafana-chinese
RUN rm -rf /usr/share/grafana/bin && rm -rf /usr/share/grafana/public
RUN cp -r /tmp/grafana-chinese/bin/linux-amd64/ /usr/share/grafana && \
  cp -r /tmp/grafana-chinese/public /usr/share/grafana
RUN mv /usr/share/grafana/linux-amd64/ /usr/share/grafana/bin/
RUN cp /usr/share/grafana/bin/grafana-server /usr/sbin/grafana-server

ENTRYPOINT ["/usr/local/bin/run-grafana.sh"]

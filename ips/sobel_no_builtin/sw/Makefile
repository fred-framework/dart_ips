# Copyright 2018 Pedro Melgueira
# Copyright 2021 Alexandre Amory. Adaptations to run with FRED/DART
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

all:
	cd src/reference/src && make && cd -
	mkdir -p build
	cd build && cmake .. && make 

run:
	convert src/reference/imgs/img.png src/reference/imgs/img.rgb
	./synthetic
	convert -size 512x512 -depth 8 src/reference/imgs/img_out.gray img_out.png

clean: 
	cd src/reference/src && make clean && cd -
	rm -f src/reference/imgs/img_out* src/reference/imgs/img.rgb
	rm -rf build

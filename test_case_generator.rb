require 'set'

MIN_NUM_TERMINI = 4
MIN_NUM_STATIONS_LINE = 4
MRT_STATION_NAMES = ["Jurong East", "Bukit Batok", "Bukit Gombak", "Choa Chu Kang", "Yew Tee", "Kranji", "Marsiling", "Woodlands", "Admiralty", "Sembawang", "Yishun", "Khatib", "Yio Chu Kang", "Ang Mo Kio", "Bishan", "Braddell", "Toa Payoh", "Novena", "Newton", "Orchard", "Somerset", "Dhoby Ghaut", "City Hall", "Raffles Place", "Marina Bay", "Marina South Pier", "Pasir Ris", "Tampines", "Simei", "Tanah Merah", "Bedok", "Kembangan", "Eunos", "Paya Lebar", "Aljunied", "Kallang", "Lavender", "Bugis", "City Hall", "Raffles Place", "Tanjong Pagar", "Outram Park", "Tiong Bahru", "Redhill", "Queenstown", "Commonwealth", "Buona Vista", "Dover", "Clementi", "Jurong East", "Chinese Garden", "Lakeside", "Boon Lay", "Pioneer", "Joo Koon", "Gul Circle", "Tuas Crescent", "Tuas West Road", "Tuas Link", "Expo", "Changi Airport", "HarbourFront", "Outram Park", "Chinatown", "Clarke Quay", "Dhoby Ghaut", "Little India", "Farrer Park", "Boon Keng", "Potong Pasir", "Woodleigh", "Serangoon", "Kovan", "Hougang", "Buangkok", "Sengkang", "Punggol", "Dhoby Ghaut", "Bras Basah", "Esplanade", "Promenade", "Nicoll Highway", "Stadium", "Mountbatten", "Dakota", "Paya Lebar", "MacPherson", "Tai Seng", "Bartley", "Serangoon", "Lorong Chuan", "Bishan", "Marymount", "Caldecott", "Botanic Gardens", "Farrer Road", "Holland Village", "Buona Vista", "one-north", "Kent Ridge", "Haw Par Villa", "Pasir Panjang", "Labrador Park", "Telok Blangah", "HarbourFront", "Bayfront", "Marina Bay", "Bukit Panjang", "Cashew", "Hillview", "Beauty World", "King Albert Park", "Sixth Avenue", "Tan Kah Kee", "Botanic Gardens", "Stevens", "Newton", "Little India", "Rochor", "Bugis", "Promenade", "Bayfront", "Downtown", "Telok Ayer", "Chinatown", "Fort Canning", "Bencoolen", "Jalan Besar", "Bendemeer", "Geylang Bahru", "Mattar", "MacPherson", "Ubi", "Kaki Bukit", "Bedok North", "Bedok Reservoir", "Tampines West", "Tampines", "Tampines East", "Upper Changi", "Expo", "Choa Chu Kang", "South View", "Keat Hong", "Teck Whye", "Phoenix", "Bukit Panjang", "Petir", "Pending", "Bangkit", "Fajar", "Segar", "Jelapang", "Senja", "Ten Mile Junction", "Sengkang", "Compassvale", "Rumbia", "Bakau", "Kangkar", "Ranggung", "Cheng Lim", "Farmway", "Kupang", "Thanggam", "Fernvale", "Layar", "Tongkang", "Renjong", "Punggol", "Cove", "Meridian", "Coral Edge", "Riviera", "Kadaloor", "Oasis", "Damai", "Sam Kee", "Teck Lee", "Punggol Point", "Samudera", "Nibong", "Sumang", "Soo Teck"]

def generate_random_graph(s, max_weight)
  Array.new(s) { |i| Array.new(s) { |j| i == j ? 0 : rand(1..max_weight) } }
end

def print_graph(matrix)
  matrix.each { |row| puts row.join(' ') }
end

def prim(matrix)
  cost = Array.new(matrix.length, Float::INFINITY)
  parent = Array.new(matrix.length, nil)
  visited = Array.new(matrix.length, false)

  # start from the first vertex
  cost[0] = 0
  parent[0] = -1

  matrix.length.times do
    u = nil
    min_weight = Float::INFINITY

    # Find unvisited vertex with minimum cost
    cost.zip(visited).each_with_index do |zipped, i|
      c, v = zipped
      if c < min_weight and !v
        min_weight = c
        u = i
      end
    end
    visited[u] = true

    matrix[u].zip(cost, visited).each_with_index do |zipped, i|
      m, c, v = zipped
      if m > 0 && !v && c > m
        cost[i] = m
        parent[i] = u
      end
    end
  end

  result = Array.new(matrix.length) { Array.new(matrix.length, 0) }

  (1...matrix.length).each do |i|
    result[i][parent[i]] = result[parent[i]][i] = matrix[i][parent[i]]
  end

  result
end

def bfs(matrix, termini)
  from, to = termini
  open_set = []
  closed_set = Set[]
  meta = {}

  root = from
  meta[root] = nil
  open_set.unshift(root)

  while !open_set.empty? do
    subtree_root = open_set.shift
    if subtree_root == to
      return construct_path(subtree_root, meta)
    end
    matrix[subtree_root].each_with_index.select { |w, i| w > 0 }.map { |x| x.last }.each do |child|
      next if closed_set.include?(child)
      if !open_set.include?(child)
        meta[child] = subtree_root
        open_set.unshift(child)
      end
    end
    closed_set.add(subtree_root)
  end
end

def construct_path(state, meta)
  result = [state]
  while !meta[state].nil? do
    state = meta[state]
    result.append(state)
  end
  result.reverse
end

# Start of main
s, max_weight = ARGV.map { |a| a.to_i }

if s <= 0 || max_weight <= 0
  puts "Invalid args"
  exit 1
end

primmed = nil
termini = []

while termini.length < MIN_NUM_TERMINI do
  graph = generate_random_graph(s, max_weight)
  primmed = prim(graph)
  termini = primmed
    .each_with_index.select do |row, i|
      row.reduce(0) { |acc, weight| acc += weight > 0 ? 1 : 0 } == 1
    end
    .map { |pair| pair.last }
end

stations = MRT_STATION_NAMES.sample(s)
popularities = Array.new(s) { rand(1..10) / 10.0 }

green_line = []
yellow_line = []
blue_line = []
while green_line.length < MIN_NUM_STATIONS_LINE || yellow_line.length < MIN_NUM_STATIONS_LINE || blue_line.length < MIN_NUM_STATIONS_LINE do
  green_termini, yellow_termini, blue_termini = termini.combination(2).to_a.sample(3)

  green_line = bfs(primmed, green_termini)
  yellow_line = bfs(primmed, yellow_termini)
  blue_line = bfs(primmed, blue_termini)
end

puts s
puts stations.join(",")
print_graph(primmed)
puts popularities.join(",")
puts green_line.map { |s| stations[s] }.join(",")
puts yellow_line.map { |s| stations[s] }.join(",")
puts blue_line.map { |s| stations[s] }.join(",")
puts 100
puts [10,10,10].join(",")

# p green_termini
# p yellow_termini
# p blue_termini
# p green_line
# p yellow_line
# p blue_line

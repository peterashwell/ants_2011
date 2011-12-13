require 'rubygems'
require 'json'

game_command = [
  './playgame.py',
  '--player_seed 42', 
  '--end_wait=0.25', 
  '--verbose', 
  #'--nolaunch',
  '--log_dir ./game_logs', 
  '--turns 1000', 
  '--turntime 5000', 
  '--map_file ../tools/maps/maze/maze_04p_01.map',
  '"$@"', 
  '"../cppbot/MyBot"', #'"./cppbot/MyBot"', 
  '"../cppbot/MyBot"', #'"python ../tools/sample_bots/python/LeftyBot.py"', 
  '"../cppbot/MyBot"', #'"python ../tools/sample_bots/python/LeftyBot.py"',
  '"../cppbot/MyBot"', #'"python ../tools/sample_bots/python/LeftyBot.py"'  
]

puts "Run: #{game_command.join(" ")}"
parallel_games = 1
games = []
(0...parallel_games).each do |i|
  puts "Spawned ##{i}"
  games << IO.popen((game_command + ["--game #{i}"]).join(" "))
end

complete_games = []
stdouts = [""] * parallel_games
while complete_games.size < parallel_games
  (0...parallel_games).each do |i|
    break if complete_games.include? i
    if games[i].eof?
      puts "Completed ##{i}"
      complete_games << i
    else
      #puts "Read ##{i}"
      games[i].read(1024)
      #stdouts[i] += games[i].read(1024)
    end
  end
end

# Games are complete... open the logs
game_results = []
(0...parallel_games).each do |i|
  file = File.open("./game_logs/#{i}.replay", "r")
  game_results[i] = JSON.parse file.gets
  puts game_results[i].keys.to_json
  ["error", "game_id", "status", "rank", "playerturns", "score"].each do |x|
    puts "#{x} = #{game_results[i][x].to_json}"
  end
  #puts game_results[i]["replaydata"]["ants"].to_json#.keys.to_json
  ants_at_end = [0] * game_results[i]["status"].size
  ants_total = [0] * game_results[i]["status"].size
  game_results[i]["replaydata"]["ants"].each do |ant|
    if ant[3] == game_results[i]["game_length"] + 1 # They survived to the end
      ants_at_end[ant[4]] += 1 
      #puts ant.to_json
    end
    ants_total[ant[4]] += 1
  end
  puts ants_at_end.to_json
  puts ants_total.to_json
  #puts game_results[i].to_json
  puts
end




<?php

define('N_COUNT', 3);
array_shift($argv);
$listCost = [];
$countFiles = count($argv);

foreach ($argv as $index => $fileName) {
    $file = basename($fileName);
    $parts = explode('.', $file);
    $file = str_replace('.' . end($parts), '', $file);
    $fileName = escapeshellarg($fileName);
    $file = escapeshellarg($file);

    `./bin/binfilter $fileName ./tmp/$file.bin`;

    $response = `./gauge.sh ./tmp/$file.bin`;
    $data = explode("\n", $response);
    $list = [];

    foreach ($data as $i => $value) {
        if ($i % 2 == 0 && !empty($value)) {
            $value = explode('/', $value);
            $list[$i >> 1]['file'] = $value[0];
            $list[$i >> 1]['path'] = implode('/', $value);
        } elseif ($i % 2 == 1) {
            $list[$i >> 1]['cost'] = floatval($value);
        }
    }

    for ($i = 0; $i < count($list); ++$i) {
        for ($j = 0; $j < count($list); ++$j) {
            if ($list[$i]['cost'] > $list[$j]['cost']) {
                $temp = $list[$i];
                $list[$i] = $list[$j];
                $list[$j] = $temp;
            }
        }
    }

    $listCost[$index] = array_slice($list, 0, N_COUNT);
}

$list = [];
$maxCostArr = [];

foreach ($listCost as $listTops) {
    foreach ($listTops as $row) {
        if (!isset($list[$row['file']])) {
            $list[$row['file']] = 0;
        }

        if (!isset($maxCostArr[$row['file']])) {
            $maxCostArr[$row['file']] = $row['cost'];
        }

        $list[$row['file']] += pow($row['cost'], N_COUNT);

        if ($maxCostArr[$row['file']] < $row['cost']) {
            $maxCostArr[$row['file']] = $row['cost'];
        }
    }
}

$maxCost = $countFiles * N_COUNT;
$listPercents = [];

foreach ($list as $file => $cost) {
    $list[$file] = $cost * pow($maxCostArr[$file], N_COUNT);
}

$result = [];

foreach ($list as $file => $row) {
    $listPercents[$file] = (float) $row / $maxCost;

    if (empty($result) || $listPercents[$file] > $result['cost']) {
        $result['file'] = $file;
        $result['cost'] = $listPercents[$file];
    }
}

echo sprintf('%s: [%s]', $result['file'], $result['cost']) . PHP_EOL;
// echo $result['file'] . $result['cost'];
// var_dump($result);
